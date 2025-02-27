---
title: "정산시스템 이벤트 기반 아키텍처(EDA)로 마이그레이션 하기"
subtitle: "이벤트 기반 아키텍처를 통한 정산시스템 개선"
date: 2024-03-03
description:
  - "레거시 정산시스템의 문제점과 이벤트 기반 아키텍처를 통한 개선"
  - "안정적인 정산 처리를 위한 시스템 마이그레이션 경험 공유"
image: images/event-driven-architecture/paywatch.png
meta_image: static/images/event-driven-architecture/paywatch.png
tags:
  - EDA
  - Migration
categories:
  - Migration
---

## 들어가며

---

&nbsp;&nbsp;&nbsp;작년 하반기에 입사당시부터 문제가 많았던 정산시스템을 마이그레이션 하였다. 회사 도메인 특성상 매달 사용자의 급여를 입금받아 정산하고 이를 다시 사용자 계좌로 입금해줘야 했다. 당시 ***매월 4,000건 50억이상의 금액***이 정산되고 있었고, 마이그레이션 과정에서 문제가 발생할 경우, 매우 크리티컬한 문제가 될 수 있었기에 회사 전체적으로도 관심을 많이 가지는 프로젝트였다. 

&nbsp;&nbsp;&nbsp;결과부터 말하면 마이그레이션은 성공적이었고, 이를 기반으로 꽤나 높은 인사고과를 받을수 있었다. 지금부터는 이 마이그레이션 과정을 정리해보려 한다.


## 기존 시스템의 문제점

---

&nbsp;&nbsp;&nbsp;기존 정산시스템의 경우 사용자에게 할당된 가상계좌로 입금이 되면 뱅킹시스템으로부터 API Callback을 받아 정산할 금액을 계산하고 이를 외부 정산 시스템에 전달(HTTP 통신)하는 구조였다. 레거시로 인해 불필요하게 세스템들이 나누어져 있었고 각 단계마다 의존성이 커 중간에 실패가 발생할 경우 이슈파악이나 처리가 어려웠다. 사용자가 늘어 나면서 급여일에는 처리누락이 다수 발생하였고, 이를 모니터링하기 위해 급여일에는 2~3명의 인원이 필요했다. 이로인해 인력 리소스 낭비가 심한 상태였다. 

{{< img src="/images/event-driven-architecture/legacy.png" caption="기존 시스템" width="500px" position="center" >}}

&nbsp;&nbsp;&nbsp;먼저 문제를 정의해보면 아래와 같다.

### 문제정의

---

1. 급여일, 많은 정산요청으로 처리 누락이나 실패가 발생
&nbsp;&nbsp;&nbsp;- 급여일에는 모놀리식 서버 스케일링 불가피

2. 이슈처리 지연 및 추적의 어려움
&nbsp;&nbsp;&nbsp;- 우리가 컨트롤할 수 없는 외부 정산시스템 처리량의 한계
&nbsp;&nbsp;&nbsp;- 지수 백오프 방식의 재처리로직 추가(처리시간 지연)
&nbsp;&nbsp;&nbsp;- 실패나 누락 발생시, 수기로 외부 시스템을 조회하여 수기로 처리

3. 외부 정산시스템과의 의존성
&nbsp;&nbsp;&nbsp;- 외부 정산시스템과 연동을 위해, 내부 로직 실행시 외부 정산시스템 데이터 동기화(HTTP 통신)
&nbsp;&nbsp;&nbsp;- 동기화 문제시, 정산 프로세스에도 영향


## 문제 해결을 위한 개선사항

---

#### 급여일, 많은 정산요청으로 처리 누락이나 실패가 발생

---

&nbsp;&nbsp;&nbsp;기존의 입금 ***Callback 방식***을 계좌조회 ***Polling 방식***으로 변경하였다. Polling 방식의 경우 Callback 방식보다 실시간성이 떨어지지만, 안정적으로 정산처리 하는것이 더 중요하다고 생각했기 때문이다. 그래서 아래 그림과 같이 Batch Job을 통해서, 사용자의 계좌 잔액을 Polling 하는 구조로 변경하여 입금에 처리에 대한 Throttling 을 내부에서 조절할수 있도록 하였다.

{{< img src="/images/event-driven-architecture/polling.png" caption="polling flow" width="500px" position="center" >}}


#### 처리속도 지연 및 이슈추적의 어려움

---

&nbsp;&nbsp;&nbsp;해당 문제를 해결하기 위해 기존의 하나의 ***거대한 트랜잭션으로 되어있던 동기 처리방식을 단계별로 나누어 이벤트로 정의***하였고(이벤트 기반 아키텍쳐), ***비동기적으로*** 처리 할 수 있도록 개선하였다. 기존에 비동기 처리를 위해 Celery를 사용하고 있었기 때문에, 이를 활용하기로 하였다. 이벤트 기반 아키텍쳐를 적용하기 위한 판단을 정리하면 아래와 같다.

{{< boxmd >}}
- 급여일과 같이 정산요청이 몰리는 경우, 이를 하나의 트랜잭션으로 처리하는게 아닌, 비동기적으로 처리하여 병목을 해결할수 있다.
- 정산과정의 트랜잭션을 분리하여 각 단계별로 이벤트로 정의하여 처리하면, 각 단계별로 처리 가능하고 이슈 추적이 쉬워진다.
- 정산 단계별 재시도(Retry)와 장애 복구(Fault Tolerance) 처리가 명확해진다.
- Eventually Consistent 모델을 적용할수 있다. (비즈니스 규칙상 정산일에는 서비스 사용이 불가능하기 때문)
{{< /boxmd >}}

&nbsp;&nbsp;&nbsp;하지만 한가지 주의해야할 점이 있었는데, 이벤트간의 순서였다. 정산처리를 위해선 몇번의 이체단계가 필요한데, 이벤트간의 순서가 지켜지지 않으면 이체가 정상적으로 동작하지 않기 때문이다. 이를 해결하기 위해서 ***FIFO 큐사용과 Zero-Payload 전략***을 차용하였다. 간단히 설명하면 이벤트 처리가 완료되면 다음 이벤트 처리를 위한 data를  db에 저장함과 동시에 FIFO 큐에 이벤트 id를 Publish 하였다. 이벤트를 Consume 하는곳에서 Publish된 이벤트 id 정보를 받아 이벤트에 해당하는 처리를 하고 db에 성공유무를 업데이트 하도록 하였다.

{{< img src="/images/event-driven-architecture/new.png" caption="event 처리 flow" width="500px" position="center" >}}

#### 외부 정산시스템과의 의존성

---

&nbsp;&nbsp;&nbsp;마이그레이션의 또 하나의 큰 과제는 외부 정산시스템을 내부로 가져오는 것이었다. 회사 초기부터 외주업체에서 관리 하고 있었고 히스토리가 거의 없었기 때문에, 해당 처리로직은 블랙박스 상태였다. 그리고 위에서 잠시 언급했듯, 회사 비즈니스 로직에서 외부정산 시스템과 데이터 동기화를 위해 HTTP 통신이 빈번하게 발생하고 있었기 때문에, 서비스의 응답성과 안정성을 크게 저하시키고 있었다. 그래서 이부분을 제로부터 설계하였고, 마이그레이션 완료후 완전히 제거하게 되었다.


## 마이그레이션 전략

---

&nbsp;&nbsp;&nbsp;마이그레이션을 진행하면서 기존에 사용하던 뱅킹시스템이 아닌 새로운 뱅킹시스템을 도입하였는데, 이 때문에 사용자 가상계좌번호 변경이 불가피 했다. 가상계좌가 변경되면서 B2B 사용자 회사의 협조도 필요했고, B2C 일반 사용자들에게는 변경된 가상계좌를 충분히 인지시킬 시간 또한 필요했다. 추가적으로 중복정산의 가능성 또한 염두해야 했다. 그래서 이를 컨트롤 하기위한 Facade Wrapper Class를 정의하여 컨트롤 하였다. 해당 Facade Wrapper Class를 간단히 작성해보면 아래와 같다.

```python
# Wrapper Class 
class BankingSelectWrapper:
    def __init__(self, user, flag) -> None:
      if condition_1:
          # 새로운 정산 시스템 logic DI
          self.selector = BankingSelector(NewBanking())
      elif condition_2:
          # 레거시 정산 시스템 logic DI
          self.selector = BankingSelector(OldBanking())
              
    def get_account_number(self):
        """가상계좌 조회"""
        return self.selector.get_account_number()

    def get_repayment_amount(self):
        """가상계좌 상환 금액"""
        return self.selector.get_repayment_amount()

    def get_suspend_amount(self):
        """가상계좌 홀딩 금액"""
        return self.selector.get_suspend_amount()
```

&nbsp;&nbsp;&nbsp;위와같이 Facade Wrapper Class를 정의하여 특정 조건이나 플래그 값을 두어 상황에 맞게 뱅킹처리의 의존성을 주입해주었고, 점진적으로 마이그레이션을 진행할 수 있었다.


## 마이그레이션 이후

---

&nbsp;&nbsp;&nbsp;마이그레이션 이후, 급여일에도 정산처리가 지연되거나 실패되는 이슈가 ***제로에 수렴***했다. 외부적인 요소(계좌정지, 예금주명 변경 등등)에 의해서 이체나 정산이 실패하는 경우에는 슬랙 알림과 함께 ***재시도 로직이나 롤백을 수행***하여 최대한 빠르게 정산이 이루어지도록 하였으며, 또한 레거시 외부시스템들의 의존성을 제거함으로써 ***매월 수백만원의 지출 감소효과***도 얻게 되었다. 


## 마치며

---

&nbsp;&nbsp;&nbsp;해당 프로젝트를 혼자 진행하는 상황이었기에 부담감이 매우 컸다. 특히 급여의 경우 사용자가 민감하게 생각하는 부분이기 때문에, 조금이라도 늦어지거나 잘못 정산되는 경우 서비스의 부정적인 인식을 심어줄 수 있었다.

&nbsp;&nbsp;&nbsp;다행히도 성공적으로 마이그레이션을 마무리했고, 회사뿐만 아니라 개인적으로도 큰 임팩트가 있는 프로젝트였다. 혼자 빠른 시간 안에 마이그레이션을 진행해야 했기에, 부족한 부분도 많고 아쉬운 부분도 남아있는데, 이러한 부분들은 앞으로 계속 개선해나갈 예정이다.