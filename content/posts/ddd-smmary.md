---
title: "DDD 기본개념 정리"
subtitle: "도메인 주도 설계로 복잡한 비즈니스 로직 관리하기"
date: 2024-08-19
description:
  - "DDD의 Strategic Design과 Tactical Design 개념 정리"
  - "복잡한 비즈니스 도메인을 효과적으로 설계하는 방법"
image: images/ddd-summary/ddd.png
meta_image: static/images/ddd-summary/ddd.png
tags:
  - ddd
  - clean code
  - architecture
categories:
  - clean code
---

## 들어가며

---

&nbsp;&nbsp;&nbsp;모놀리식 서비스가 점점 거대해지면서 비즈니스 로직의 복잡성이 증가하는 경우가 많다. 특히 복잡한 도메인에서는 여러 비즈니스 규칙들이 얽혀있어 코드의 가독성과 유지보수성이 크게 떨어지는 상황이 발생한다. 이런 문제를 해결하기 위해 DDD(Domain Driven Design)에 대해 학습하게 되었고, 그 내용을 정리해보려 한다.

&nbsp;&nbsp;&nbsp;DDD는 단순히 코딩 기법이 아닌, 복잡한 비즈니스 도메인을 효과적으로 관리하기 위한 설계 철학이다. **Loose Coupling, High Cohesion** 원칙을 기반으로 도메인의 복잡성을 관리하고, 비즈니스 요구사항을 코드로 명확하게 표현하는 것이 핵심이다.

## DDD란 무엇인가?

---

&nbsp;&nbsp;&nbsp;**Domain**이란 유사한 업무의 집합으로, 소프트웨어로 문제를 해결하고자 하는 영역을 의미한다. 예를 들어 온라인 쇼핑몰에서는 '주문', '상품 관리', '고객 관리', '배송' 등이 각각의 도메인이 될 수 있다.

&nbsp;&nbsp;&nbsp;전통적인 개발 방식에서는 기술적 관점(데이터베이스, UI, 비즈니스 로직)으로 시스템을 나누는 경우가 많다. 하지만 DDD는 **비즈니스 도메인을 중심**으로 시스템을 설계하는 접근법이다. 이를 통해 개발자와 도메인 전문가(기획자, 비즈니스 담당자) 간의 소통을 원활하게 하고, 비즈니스 요구사항을 코드에 명확하게 반영할 수 있다.

&nbsp;&nbsp;&nbsp;DDD는 크게 두 가지 단계로 나뉜다:

- **Strategic Design**: 개념적인 설계 단계 - 큰 그림을 그리는 단계
- **Tactical Design**: 실제 구현을 위한 설계 단계 - 코드 레벨의 구체적인 설계

## Strategic Design (전략적 설계)

---

### Ubiquitous Language 

&nbsp;&nbsp;&nbsp;모든 현업자들이 효율적인 의사소통을 위해 동일한 표현과 단어를 사용하는 것이다. 이는 DDD의 가장 중요한 개념 중 하나로, 코드의 변수명, 클래스명, 메서드명까지 모두 이 공통 언어를 기반으로 작성한다.

&nbsp;&nbsp;&nbsp;**왜 중요한가?** 개발자가 "User"라고 부르는 것을 기획자는 "고객", 마케팅팀은 "회원"이라고 부른다면 소통 과정에서 오해가 발생할 수 있다. 공통 언어를 정의하면 이러한 혼란을 방지하고, 코드를 읽는 것만으로도 비즈니스 로직을 이해할 수 있게 된다.

&nbsp;&nbsp;&nbsp;예를 들어:
- 할인 ❌ → 프로모션 ✅ 
- 만료 ❌ → 종료일 ✅ 
- 사용자 ❌ → 고객 ✅ 

### Domain Model과 서브도메인

&nbsp;&nbsp;&nbsp;**Domain Model**은 유사한 업무의 집합을 추상화한 것으로, 객체모델이나 상태다이어그램 등 다양하게 표현할 수 있다. 이는 실제 비즈니스 세계의 개념, 규칙, 프로세스를 소프트웨어 모델로 표현한 것이다.

&nbsp;&nbsp;&nbsp;모든 도메인이 동일한 중요도를 갖지는 않는다. 도메인 모델은 크게 세 가지 서브도메인으로 분류된다:

- **Core Domain**: 비즈니스 목적 달성을 위한 핵심 도메인 (주문, 결제)
- **Supporting Domain**: 핵심 도메인을 지원하는 도메인 (포인트, 쿠폰)
- **Generic Domain**: 공통 기능 도메인 (알림, 인증)

### Bounded Context 

&nbsp;&nbsp;&nbsp;특정 비즈니스 영역의 경계를 의미한다. 이는 DDD에서 가장 중요한 개념 중 하나로, **한 팀이 관리할 수 있는 도메인 모델의 경계**를 정의한다. 

&nbsp;&nbsp;&nbsp;대규모 시스템에서는 모든 것을 하나의 거대한 모델로 관리하기 어렵다. 같은 용어라도 컨텍스트에 따라 다른 의미를 가질 수 있기 때문이다. 예를 들어 '상품' 도메인의 경우:

- **주문 컨텍스트**: 주문 가능한 상품 정보 (재고, 가격, 배송 정보)
  - 상품의 실제 구매 가능 여부와 물리적 특성에 집중
  - 재고 관리, 가격 정책이 핵심 관심사

- **카탈로그 컨텍스트**: 고객에게 보여줄 상품 정보 (이미지, 설명, 카테고리)
  - 상품의 마케팅적 특성과 고객 경험에 집중
  - 검색, 추천, 상품 상세 정보가 핵심 관심사

&nbsp;&nbsp;&nbsp;각 Bounded Context는 **독립적인 모델**을 가지며, 서로 다른 데이터베이스를 사용할 수도 있다. 하나의 Bounded Context는 보통 하나의 팀이 관리하며, MSA 환경에서는 하나 이상의 마이크로서비스로 구현된다.

### Context Map

&nbsp;&nbsp;&nbsp;Bounded Context들 사이의 관계를 도식화한 것이다. 실제 개발에서는 여러 컨텍스트가 서로 상호작용해야 하는데, 이 관계를 명확히 정의하는 것이 중요하다. 주요 패턴들은 다음과 같다:

- **Upstream-Downstream**: 
  - 데이터의 흐름 방향성을 나타냄 (U → D)
  - Upstream은 데이터를 제공하는 쪽, Downstream은 데이터를 소비하는 쪽
  - 예: 주문 시스템(U) → 배송 시스템(D)

- **Anticorruption Layer**: 
  - 서로 다른 모델 간의 변환을 담당하는 계층
  - 외부 시스템의 복잡한 모델이 내부 도메인을 오염시키지 않도록 보호
  - 레거시 시스템과 연동할 때 특히 유용

- **Open Host Service**: 
  - 다른 컨텍스트들이 사용할 수 있는 공개 API 제공
  - RESTful API, GraphQL 등으로 구현
  - 여러 downstream 시스템이 사용할 수 있는 안정된 인터페이스

- **Shared Kernel**: 
  - 두 개 이상의 컨텍스트가 공통 모델을 공유
  - 코드 중복을 줄일 수 있지만, 변경 시 협의가 필요
  - 신중하게 사용해야 하는 패턴

## Tactical Design (전술적 설계)

---

&nbsp;&nbsp;&nbsp;Strategic Design에서 도출한 개념을 실제 개발 가능한 수준까지 구체화하는 단계다.

### 핵심 구성 요소

#### Entity 
- **Mutable 객체**로 식별자(ID)를 통해 구별되는 도메인 객체
- 시간이 지나면서 상태가 변할 수 있음 (예: 주문 상태 변경)
- 비즈니스 규칙이나 로직이 포함됨
- **연속성(Continuity)**이 중요한 객체들
- 예: User, Order, Product - 각각 고유한 ID를 가지고 상태가 변함

#### Value Object
- **Immutable 객체**로 속성값으로 비교되는 객체
- 식별자가 없고, 값 자체가 의미를 가짐
- Entity의 속성으로 사용되거나 독립적으로 사용됨
- 생성 후 변경되지 않으며, 변경이 필요하면 새로운 객체를 생성
- 예: Money(amount, currency), Address(street, city, zipCode), Email

#### Aggregate
- **Entity와 VO의 집합**으로 하나의 Root Entity(Aggregate Root)를 포함
- 복잡한 도메인 객체를 관련된 것끼리 그룹핑한 **일관성 경계**
- **비즈니스 불변조건**을 유지하는 단위
- 외부에서는 반드시 Aggregate Root를 통해서만 접근 가능
- **트랜잭션 경계**: 한 트랜잭션에서는 하나의 애그리거트만 수정
- 크기를 작게 유지하는 것이 성능과 확장성에 유리

```python
class Address:  # Value Object
    def __init__(self, street, city, zip_code):
        self.street = street
        self.city = city
        self.zip_code = zip_code

class OrderItem:  # Entity
    def __init__(self, product_id, quantity, price):
        self.product_id = product_id
        self.quantity = quantity
        self.price = price

class Order:  # Aggregate Root
    def __init__(self, order_id, customer_id, shipping_address):
        self.order_id = order_id
        self.customer_id = customer_id
        self.shipping_address = shipping_address
        self.order_items = []
        self.status = "Created"

    def add_item(self, product_id, quantity, price):
        item = OrderItem(product_id, quantity, price)
        self.order_items.append(item)

    def confirm_order(self):
        self.status = "Confirmed"
```

### 기타 중요 구성 요소

#### Domain Service
- **여러 Entity나 VO에 걸쳐 있는 비즈니스 로직**을 처리
- 특정 Entity에 속하지 않는 도메인 로직을 담당
- 예: 계좌 이체 서비스(두 계좌 Entity 간의 상호작용), 가격 계산 서비스

#### Repository
- **도메인 객체의 영속성을 관리**하는 인터페이스
- 도메인 계층에서는 인터페이스만 정의하고, 인프라 계층에서 구현
- 메모리 컬렉션처럼 사용할 수 있는 추상화를 제공
- 예: `UserRepository.findById()`, `OrderRepository.save()`

#### Application Service
- **유스케이스를 구현**하는 계층
- 도메인 객체들을 조합하여 비즈니스 플로우를 처리
- 트랜잭션 경계를 관리
- 도메인 로직은 포함하지 않고, 오케스트레이션만 담당

## 아키텍처 패턴

---

### Layered Architecture
전통적인 계층형 아키텍처로 **표현 → 응용 → 도메인 → 인프라** 순으로 구성된다.

- **표현 계층**: UI, 컨트롤러 - 사용자 인터페이스와 외부 요청 처리
- **응용 계층**: 애플리케이션 서비스 - 유스케이스 구현 및 트랜잭션 관리
- **도메인 계층**: 엔티티, 값 객체, 도메인 서비스 - 핵심 비즈니스 로직
- **인프라 계층**: 데이터베이스, 외부 API - 기술적 구현 세부사항

**장점**: 간단하고 이해하기 쉬움  
**단점**: 상위 계층이 하위 계층에 의존하여 테스트가 어려울 수 있음

### Hexagonal Architecture
도메인을 중심에 두고 외부 의존성을 분리하는 아키텍처다.

- **도메인이 중심**에 위치하며 외부 의존성으로부터 완전히 분리
- **Port**: 외부와 소통하기 위한 인터페이스 (도메인 계층에서 정의)
- **Adapter**: Port 인터페이스의 실제 구현체 (인프라 계층에서 구현)
- **의존성 역전**: 모든 의존성이 도메인 쪽으로 향함

&nbsp;
&nbsp;**핵심중점** :
- 비즈니스 로직(도메인)을 기술적 세부사항(DB, 웹 프레임워크)으로부터 완전히 분리
- 테스트하기 쉽고, 기술 스택 변경에 유연하게 대응 가능
- 도메인 로직에 집중할 수 있는 환경 제공


## 마치며

---

&nbsp;&nbsp;&nbsp;DDD는 단순한 개발 방법론이 아닌, **복잡한 비즈니스 도메인을 효과적으로 관리하기 위한 철학**이다. Strategic Design을 통해 Context Map을 도출하고, 각 Bounded Context 안에서 도메인의 Entity, VO, Aggregate를 식별한 후, Tactical Design 단계에서 이를 실제 코드로 구현하는 것이 핵심이다.

&nbsp;**DDD의 핵심 가치**는 다음과 같다:
- **도메인 전문가와 개발자 간의 지식 공유**: 공통 언어를 통한 효과적인 소통
- **복잡성 관리**: 큰 문제를 작은 경계로 나누어 관리
- **비즈니스 중심 설계**: 기술이 아닌 비즈니스 가치에 집중
- **지속 가능한 소프트웨어**: 변화하는 비즈니스 요구사항에 유연하게 대응

&nbsp;
&nbsp;특히 **복잡한 비즈니스 규칙이 많은 도메인**에서는 DDD의 접근 방식이 매우 유용하다. 단순한 CRUD 애플리케이션보다는 복잡한 비즈니스 로직, 지속적인 변화가 있는 프로젝트에서 사용한다면 도메인을 명확히 하여 코드의 가독성을 높여주고, 지속적인 변화에 유연한 구조를 가지게 된다.