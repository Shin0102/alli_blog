---
title: "AWS DMS를 활용한 NoSQL 적용기"
subtitle: "AWS DMS를 활용한 NoSQL 적용기"
date: 2024-05-27
description:
  - "AWS DMS CDC"
  - "NoSQL"
image: images/nosql-apply-using-aws-dms/aws-dms.png
meta_image: static/images/nosql-apply-using-aws-dms/aws-dms.png
tags:
  - aws
  - dms
  - cdc
  - nosql
categories:
  - 성능개선
---

## 들어가며

---

&nbsp;&nbsp;&nbsp;최근 회사에서 대량의 데이터를 엑셀로 다운로드하는 기능의 성능개선 요청이슈가 있었다. 워낙 많은양의 데이터를 한번에 불러오는 작업이다보니 DB에 병목현상이 발생하였다. DB로는 MySQL을 사용하고 있었는데, 기존의 DB 정규화가 과도하게 되어있는데다가 다운로드 해야할 데이터가 워낙 많아서 쿼리최적화나 스케일링으로 드라마틱한 효과를 볼 수 없을거 같았다. 또한 역정규화나 이벤트기반 데이터 싱크방법을 생각해봤지만, 다른 프로젝트도 진행해야 하는 상황에서 많은 시간을 할당할 수 없었고 기존 코드 수정은 최대한 피하고 싶었다.

&nbsp;&nbsp;&nbsp;문제 해결을 위한 방법을 찾던중, AWS의 DMS를 알게 되었다. 이를 활용하여 CDC 데이터를 NoSQL로 마이그레이션 해보기로 결정하였고, 빠르게 PoC를 진행하고 Production 까지 배포할 수 있었다. 그 결과 **기존에 비해 속도가 수십배 가량 향상되는 효과**를 얻을 수 있었다.

## 기존 시스템의 한계

---

&nbsp;&nbsp;&nbsp;현재 백엔드는 Django Monolithic Server와 MySQL DB(+Redis)로 심플한 구조로 운영되고 있었다. 백오피스에 제공되는 API 또한 Django Server의 ORM을 통해 제공되고 있었는데, 쿼리 최적화를 통해 어느정도 성능개선을 했지만 원하는 수준까지 개선하기에는 한계가 있었다. 상환업무를 위해서는 대량+다량의 데이터가 필요하기 때문이다. 또한 수십개의 테이블의 데이터를 ORM을 통해 가져오다보니 코드의 가독성이나 유지보수 측면에서도 좋지 않았다. 그래서 몇가지 해결책을 고민해보았다.

1. **DB Scaling**
2. **Raw SQL or BI 툴 활용**  
3. **Batch를 통해 Dashboard table 구성**

&nbsp;
&nbsp;&nbsp;&nbsp;위의 3가지 방법 해결책을 고민했지만 모두다 만족할만한 해결책은 아니었다. 먼저 DB Scaling의 경우 현재 Django ORM 구조에서 한계점을 느꼈기 때문에, 가격대비 극적인 성능개선은 어려울것으로 판단했다.

&nbsp;&nbsp;&nbsp;두번째로 Raw SQL이나 BI 툴 활용하는 것인데, 현재 데이터 팀이 따로 없기 때문에 백엔드 팀에서 복잡한 쿼리를 작성하거나 데이터 파이프라인을 구축하기에는 무리가 있었다.

&nbsp;&nbsp;&nbsp;마지막으로 Batch를 통한 Dashboard table 구성인데, 좋은방법이지만 Batch 주기를 짧게하기엔 기존시스템에 무리가 갈것이고, Batch 주기를 너무 길게하면 데이터의 신뢰성이 떨어질것으로 판단했다(물론 실시간성이 중요한 데이터는 아니었다). 그리고 **최대한 기존 시스템에 영향이 없는 방향**으로 작업하고 싶은 니즈가 있었다.

&nbsp;&nbsp;&nbsp;그래서 최종적으로 AWS Summit을 통해 알게된, 'NoSQL 전환을 통해 데이터 읽기속도를 개선한 사례'를 보고 실제 서비스에 적용을 해보기로 하였다. (해당 영상: https://www.youtube.com/watch?v=R4-mC4D_JN4)

## MySQL → NoSQL 마이그레이션 Flow

---

![MySQL to NoSQL Migration Flow](/images/nosql-apply-using-aws-dms/dms-flow.png)

&nbsp;&nbsp;&nbsp;구성한 flow는 위 그림과 같다.

1. **MySQL DB의 CDC 데이터를 DMS를 통해 S3로 적재**
2. **S3에 데이터가 적재되면 Lambda 트리거**  
3. **Lambda에서 데이터를 가공하여 NoSQL DB에 저장**

&nbsp;
&nbsp;&nbsp;&nbsp;여기서 한가지 의문점이 생길수 있다. 왜 CDC 데이터를 바로 NoSQL DB에 저장하지 않고 딜레이가 발생할수 있는 'CDC S3 → Lambda' 플로우를 추가했을까? 정리하면 아래와 같다.

- 테이블 설계는 확장성을 위해 과도하게 정규화 되어있거나 테이블의 row 데이터를 활용하는 케이스가 다수 존재하였다.
- 이를 그대로 NoSQL로 적재하면 성능개선에는 효과는 있겠지만, 쿼리가 다소 복잡하여 사용성이 떨어지질거라 생각하였다. (DMS에 Transformation rule을 설정할수 있지만 한계점이 있었다)
- 그래서 CDC 데이터를 Lambda를 통해 가공하여 **NoSQL DB에 저장되는 Collection의 개수를 줄여 단순화** 하고자 하였다.

&nbsp;&nbsp;&nbsp;백오피스의 보여지는 데이터들은 *비교적 변경이적이고 실시간성이 크게 중요하지 않았기 때문에 위와 같은 결정을 할수있었다. (그렇다고 delay가 엄청긴것은 아니였으며, 현재 데이터 변경부터 NoSQL DB 저장까지 1분 이내에 반영되고 있다.)

## TroubleShooting

---

&nbsp;&nbsp;&nbsp;이번엔 위에 같은 컨셉을 가지고 PoC부터 실제 실서버에 적용하기까지 겪었던 TroubleShootting을 정리해보도록 하겠다.

&nbsp;&nbsp;&nbsp;먼저 DMS 설정부분이다. AWS DMS Flow를 적용하려면 아래와 같이 세가지의 설정이 필요하다.

1. **DMS Migration Task를 실행할 EC2 인스턴스**
2. **Source Endpoint**
3. **Target Endpoint**

&nbsp;
&nbsp;&nbsp;&nbsp;첫번째로 Migration EC2 인스턴스는 스펙을 충분히 큰걸로 생성해야한다. 실제 프로덕션의 데이터에 따라 인스턴스 스펙이 부족하면 마이그레이션 도중 실패해버리게 된다. 그러므로 인스턴스를 모니터링 하면서 마그레이션에 실패하는지 체크하고 충분한 스펙의 인스턴스로 업그레이드 해줘야 한다. 그리고 Endpoint 설정 또한 꼼꼼히 체크해봐야한다. DMS에서는 다양한 Endpoint를 설정할 수 있는데, 이에 대한 설정가이드를 공식문서를 통해 확인하고 설정해야 마이그레이션 실패를 줄일수 있다. (가이드 링크: https://docs.aws.amazon.com/dms/latest/userguide/CHAP_Endpoints.html)

&nbsp;&nbsp;&nbsp;두번째로는 DMS Full Load 문제다. 초기 마이그레이션시 기존데이터의 마이그레이션이 필요하기때문에 Full load 옵션을 세팅하여 DMS를 설정하게 된다. 말 그대로 지정한 Source Endpoint의 데이터를 모두 불러오기 때문에, DMS의 'Selection Rule'을 통해 마이그레이션이 필요한 테이블 + 필터를 적절하게 지정해주는것이 좋다. 추가적으로 Full Load 시, 'load-order' 옵션을 통해서 로드순서를 지정할수 있는데, 관계형 DB에서는 데이터를 로드하는 순서가 중요하기때문에 'load-order'를 추가하여 테이블의 데이터가 로드되는 순서를 지정할 수 있다. ('load-order'의 효과를 얻기위해서는 'Maximum number of tables to load in parallel'의 설정을 1로 설정하여 병렬처리를 제한해야 한다.)

&nbsp;&nbsp;&nbsp;아래는 MySQL Selection Rule에 간단한 예시다.
(테이블1을 로드하고 테이블2를 로드함, 테이블2는 특정 column and 필터가 적용되어 있음)

```json
{
  "rules": [
    {
      "rule-type": "selection",
      "rule-id": "1",
      "rule-name": "1",
      "load-order": 1,
      "object-locator": {
        "schema-name": "해당 schema 명",
        "table-name": "테이블1"
      },
      "rule-action": "include",
      "filters": [],
      "parallel-load": null,
      "isAutoSegmentationChecked": false
    },
    {
      "rule-type": "selection",
      "rule-id": "2",
      "rule-name": "2",
      "load-order": 2,
      "object-locator": {
        "schema-name": "해당 schema 명",
        "table-name": "테이블2"
      },
      "rule-action": "include",
      "filters": [
        {
          "filter-type": "source",
          "column-name": "filter_column",
          "filter-conditions": [
            {
              "filter-operator": "eq",
              "value": "filter value1"
            },
            {
              "filter-operator": "eq",
              "value": "filter value2"
            }
          ]
        }
      ],
      "parallel-load": null,
      "isAutoSegmentationChecked": false
    }
  ]
}
```

&nbsp;&nbsp;&nbsp;마지막으로, 설계상 놓쳤던 부분인데 CDC 데이터를 S3에 적재하고 이를 Lambda로 트리거 하도록 하는것은 좋지 못한 선택이었다. 위에서도 언급했듯이, Source Endpoint가 RDS인 경우 **데이터를 로드하는 순서가 중요한데 해당 플로우는 로드순서를 보장해주지 못한다**.('load-order'는 Full Load시에만 적용되는 옵션이었음…) 그래서 **재시도로직을 추가해서 대응**하였고 정상적으로 동작함을 확인하였다.(물론 재시도로직 때문에 추가적인 delay가 발생하는것은 피할수 없었다) 이 부분은 추후 **데이터 스트림이나 Queue 시스템을 활용**하도록 변경할 예정이다.

## 도입효과

---

&nbsp;&nbsp;&nbsp;성능측정 결과 **기존 대비 수십배 이상 빨라지는 효과**를 얻을 수 있었고, 그 결과 상환이 많이 몰리는 10, 25일 급여일의 상환대상자 정보를 훨씬 빠르게 조회할수 있게 되었다. 또한 기존의 복잡한 ORM 쿼리대신 간단한 쿼리 몇줄로 모든 데이터를 불러올수 있게 되어 **코드의 가독성 또한 크게 개선**되었다.

## 마치며

---

&nbsp;&nbsp;&nbsp;이 작업은 다음 프로젝트에 투입되기 전에 잠시 시간을 내어 작업했던 것이다. 처음에는 간단한 PoC로 진행하였었던 것인데, 실제 프로덕션까지 적용하게 되었다. 처음 도입하는 플로우다보니 생각보다 시간을 많이 쓰게 되었고 설계 측면에서 아쉬운 점이 있었지만, 결과적으로 성과를 내어 보람을 느낄수 있었다.

&nbsp;&nbsp;&nbsp;현재 서비스가 성장하면서 비즈니스 로직이 점점 복잡해지고 성능과 코드 가독성 개선에 한계를 점점 느끼고 있었는데. 이번에 새롭게 MongoDB를 도입함으로써, 앞으로 문제 해결을 위해 MongoDB를 적극적으로 활용할 수 있을 것 같다.