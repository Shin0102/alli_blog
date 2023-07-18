---
title: "일체형 데이터베이스, 목적에 맞게 MSA 구조로 전환하기"
subtitle: "일체형 데이터베이스, 목적에 맞게 MSA 구조로 전환하기"
date: 2023-05-01
description:
  - ""
image: images/aws-summit-2022/aws-summit-2022.png
meta_image: static/images/aws-summit-2022/aws-summit-2022.png
tags:
  - aws summit
  - msa
  - database
categories:
  - msa
---

## 들어가며

---

&nbsp;&nbsp;&nbsp;현재 회사에서 Monolothic Django로 서비스를 운영중이다. Django Admin, 비동기 처리를 위한 Worker 서버는 다른 Elastic Beanstalk 서버로 운영중이지만 같은 DB, 소스코드를 공유중인 상태로 의존성이 매우 큰 상태이다. 현재 Monolothic Django가 굉장히 거대해졌고, 백오피스 운영에 필요한 수많은 Table join과 월급날 상환 이벤트로인해 이슈들이 늘어가고 있는 상황이다. 이 과정에서 MSA를 고민하게 되었고 그러던중 좋은 영상이 있어 정리하게 되었다. MSA를 위해서는 도메인을 적절히 나누고 그에 맞는 Database를 선택 해줘야 하는데 영상에는 여러 CASE에 대한 MSA 구성에 대해 아주 잘 정리되어 있었다. 아래는 영상을 기반으로 정리한 내용이다.

**영상 URL**: <a href="https://www.youtube.com/watch?v=R4-mC4D_JN4" target="_blank">일체형 데이터베이스, 목적에 맞게 MSA 구조로 전환하기</a>

## CASE1. 고객 CRM 시스템

---

{{< img src="images/aws-summit-2022/msa-case1.png" title="고객 CRM 시스템 분리" caption="NoSQL" position="center" width="500px" >}}
&nbsp;&nbsp;&nbsp;첫번째 CASE는 현재 고민중이기도 한 내용이었다. 현재 Django를 활용해서 백오피스를 운영중인데 많은 정보를 제공해줘야 하다보니 Table join이 늘어나고, 코드 또한 쿼리 최적화를 위한 ORM 코드를 작성해야 되기때문에 가독성 또한 많이 나빠졌다. 이러한 상황을 해결하기 위해서 영상에서는 DMS를 활용하여 RDB 에서 NoSQL(Document Data)로 마이그레이션 하였다. 이 과정을 통해서 Table join 과정을 줄이고 코드 또한 간단하게 수정하는 예제를 소개하고 있다. 코드를 리팩토링하는 것이 아닌 DB를 변경하는 것으로도 코드를 간단하게 만드는 관점은 굉장히 흥미로웠다.

## CASE2. 리더보드(실시간) 구성

---

{{< img src="images/aws-summit-2022/msa-case2.png" title="리더보드(실시간) 구성" caption="Redis" position="center" width="500px" >}}
&nbsp;&nbsp;&nbsp;실시간 랭킹같이 많은 데이터를 조회해서 보여줘야 하는경우, 이를 위한 쿼리가 시스템 자원을 많이 사용하기 때문에 사용자의 요청을 지연시키는 원인이 된다. 따로 batch를 통해서 리더보드 테이블을 업데이트하는 방법을 생각했지만, 영상에서는 Redis Sorted Set을 활용하여 요구사항을 충족시키는 예제를 소개하였다.

## CASE3. 한정판매 이벤트 처리하기

---

{{< img src="images/aws-summit-2022/msa-case3.png" title="한정판매 이벤트 처리" caption="Redis" position="center" width="500px" >}}
&nbsp;&nbsp;&nbsp;Database에서 특정 Database block에 대한 요청이 늘어나게 되면 hot block이 발생하고 이에 따라 사용자들의 요청에 대한 처리를 느리게 한다. 특히나 영상에서 나온 예제와 같이 한정판매와 같은 이벤트는 많은 사용자가 동시에 요청하게 되므로 hot block이 발생할 가능성이 높아진다. 그림 왼쪽에서 처럼 RDB를 사용하게 되면 상품에 대한 수량이 업데이트 될때 Data Row가 Locking이 되게 되므로 성능저하에 원인이 된다. 이를 해결하기 위해서 한정판매 테이블을 Redis Collection으로 변경하였다. 이렇게 하면 사용자 요청 프로세스 간의 경합이 줄어들게 될고, 성능이 개선 될 수 있다. 위와 같은 변경으로 약 4배가량의 성능 향상이 있었다고 한다.

## CASE4. 구매내역 조회 서비스

---

{{< img src="images/aws-summit-2022/msa-case4.png" title="구매내역 조회 서비스" caption="DynamoDB" position="center" width="500px" >}}
&nbsp;&nbsp;&nbsp;네번째 Case는 첫번째 Case와 거의 동일하므로 따로 정리하지 않았다. 데이터 형태에 따라 적절한 DB를 선택하는 의사결정이 중요하다는 것을 강조하였다.

## 마치며

---

&nbsp;&nbsp;&nbsp;영상을 보니 실제 사례들을 통해서 어떤 Database를 선택하고 실제로 어떻게 구성하는지 까지 Case와 예제까지 보여주어서 좋았다. 물론 서비스를 운영하거나 개발하면서 'A일때 B이고, C일때 D이다'라는 정답은 없겠지만, MSA 초기단계에 있는 서비스들에게는 꽤나 큰 도움이 될만한 영상인것 같다. 마지막으로 목적별 데이터베이스에 관한 자료를 끝으로 글을 마무리 한다.
{{< img src="images/aws-summit-2022/msa-database.png" title="MSA" caption="Database" position="center" width="700px" >}}
