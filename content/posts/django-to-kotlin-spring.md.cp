---
title: "Django에서 DDD로: Kotlin + Spring 마이그레이션"
subtitle: "레거시 Django 구조를 DDD + Event 기반으로 재설계"
date: 2025-09-15
image: images/django-to-kotlin-spring/thumbnail.png
meta_image: static/images/django-to-kotlin-spring/thumbnail.png
description:
  - "Django 기반 레거시를 Kotlin + Spring으로 옮기며 DDD와 이벤트 기반 구조로 재설계한 경험"
  - "설계(Event 도출, Aggregate)와 Axon 기반 구현, 트랜잭션·운영 측면에서의 시행착오"
tags:
  - DDD
  - Kotlin
  - SpringBoot
  - Axon
  - EventSourcing
categories:
  - backend
  - architecture
  - migration
---

## 들어가며

---

&nbsp;&nbsp;&nbsp;최근 Django 기반 레거시 시스템의 일부를 Kotlin + Spring Boot로 마이그레이션했다.  
단순한 언어 전환이 아니라, **DDD + 이벤트 기반 구조로 재설계하는 작업**이었다.

프로젝트가 어느 정도 마무리되면서, 진행하면서 경험한 내용을 정리해보려 한다.

- Django 구조에서 한계였던 점
- DDD를 적용하면서 설계가 어떻게 바뀌었는지
- 이벤트 기반 구조로 전환하며 얻은 것과 어려웠던 점

### 왜 Kotlin + Spring Boot?

&nbsp;&nbsp;&nbsp;기존 레거시는 Django 기반 API 서버로 운영되고 있었다. Django는 생산성과 빠른 개발이라는 장점이 있다. 하지만 도메인이 커지면서 아래와 같은 문제가 드러났다.

1. 비즈니스 로직의 파편화 및 중복
    - Django ORM은 모델과 DB가 직접 맞닿는 Active Record 패턴이다. 모델이 스키마에 묶이면서 유스케이스 단위로 응집하기 어렵고, 실제 도메인과 모델이 1:1로 매핑되지 않는 경우가 많아졌다. 서비스 레이어가 비대해지며 로직이 여기저기 흩어지고 중복되었다.
    - 회사 내부 컨벤션으로 완화를 시도했고 유지보수에는 분명 도움이 됐지만, 여전히 “비즈니스가 어디에 모여 있는지”를 한눈에 보기는 어려웠다.
2. 복잡한 레거시 로직
    - 스타트업 특성상 빠르게 피처를 내야 했고, 컨벤션 이전에 다양한 스타일로 쌓인 코드가 섞여 있었다. 그 코드를 다시 재사용하는 일이 잦아지면서 복잡도가 누적됐다.
3. 확장성 및 성능
    - 데이터를 DB 구조 중심으로만 끌고 가다 보니 관련 테이블이 늘고, 조회 시 조인이 잦아졌다. 성능과 가독성이 함께 나빠졌다.
    - 핵심 도메인 코드에 의존하는 코드가 많아지면서 모듈 간 경계도 흐려졌다.

&nbsp;
&nbsp;&nbsp;&nbsp;그래서 팀은 DDD로 도메인을 다시 세우자는 쪽으로 방향을 잡았다. **언어나 프레임워크가 DDD를 막는다고 단정하기는 어렵지만**, 우리 팀 상황에서는 컨벤션만으로는 한계가 분명했고, 정적 타입·계층 나누기·트랜잭션 경계를 코드로 표현하기에 Kotlin과 Spring 생태계가 더 맞다고 판단했다. 비즈니스도 어느 정도 안정권에 들어와, “빠른 개발”보다 “안정적 운영과 명확한 도메인” 쪽 무게가 커진 시점이었다. Kotlin + Spring Boot를 고른 이유는 대략 다음과 같다.
- 정적 타입과 명확한 패키지·계층 구조
- 도메인 중심 설계를 코드 구조로 옮기기 쉬운 관례
- 트랜잭션과 경계를 선언적으로 다루기 좋음
- 레퍼런스와 생태계가 넓음
- LLM 으로 생산성 보완

## 설계

---

&nbsp;&nbsp;&nbsp;개발에 앞서 회사 비즈니스 로직을 다시 정리할 필요가 있었다. 더 이상 쓰이지 않는 흐름도 있었고, 비교적 최근에 합류한 팀원은 전체 코드 구조를 아직 다 파악하지 못한 상태였다. 전원이 약 한 달간 코드를 따라가며 도메인에서 일어나는 **이벤트를 도출**하는 방식으로 시작했고, 도출된 이벤트를 바탕으로 Entity와 Value Object를 정의한 뒤, 응집도가 큰 단위끼리 묶어 Aggregate로 잡았다. Bounded Context의 경우 실제 배포 모듈 단위로 사용 예정이라, 팀 규모를 감안해 지나치게 잘게 쪼개지는 않기로 했다.

&nbsp;&nbsp;&nbsp;처음에는 DDD 용어와 흐름에 러닝 커브가 있었다. 다만 코드 레벨에서 나오는 이벤트를 하나씩 정리하다 보니 Entity·Value Object 매핑이 자연스럽게 잡혔고, Aggregate 정의까지 속도가 붙었다. 흩어져 있던 로직이 엔티티나 값 객체 단위로 모이면서, 도메인을 중복 없이 정리할수 있었다.

## 구현

---

&nbsp;&nbsp;&nbsp;팀원 모두가 Kotlin + SpringBoot 프로젝트가 모두 처음이라 관련 레퍼런스를 찾아보거나 인터넷 강의를 같이 들으며, 프로젝트 셋업을 해보고 간단한 기능을 만들어보면서 기본적인 토대를 만들었다. 프로젝트 구조는 아래와 같다.
```
eventsourcing/
├── common/              # 공통 설정 (Axon·Kafka·Jackson·암호화)
├── <도메인>/            # Bounded Context별 도메인 구현 모듈
│   └── src/main/kotlin/<도메인>/
│       ├── domain/aggregate/    # Aggregate (도메인 상태·불변식)
│       ├── application/         # Command/Event Handler, port
│       └── adapter/inbound/     # Kafka consumer, REST
├── <도메인>-api/        # 해당 도메인의 Command·Event·DTO 정의
│   ⋮                    # 도메인이 늘면 (<도메인>, <도메인>-api) 쌍이 추가된다
└── admin/               # 운영 어드민 (DLQ 재처리, 조회)
```
&nbsp;&nbsp;&nbsp;프로젝트는 Gradle 멀티모듈로 구성했고, 크게 세 부분으로 나뉜다.

- `common` — Axon·Kafka·Jackson·암호화 등 **공통 설정**을 모아 둔 모듈
- `<도메인>` — 각 Bounded Context의 **도메인 구현 모듈** (Aggregate, Command/Event Handler, Projection)
- `<도메인>-api` — 해당 도메인의 **Command·Event·DTO 정의만 담은 라이브러리 모듈**
- `admin` — 운영용 어드민 (DLQ 재처리, 데이터 조회등)

&nbsp;
&nbsp;&nbsp;&nbsp;`-api` 모듈을 따로 뗀 이유는 Axon이 메시지(Command·Event) 기반으로 동작하기 때문이다. 이렇게 한 이유는 이벤트를 발행하는 쪽, 소비하는 쪽, 어드민까지 여러부분에서 공유하기 때문이다. 무거운 구현 모듈 전체에 의존하게 두면 결합도가 올라가서, 따로 정의만 담은 가벼운 `-api` 모듈을 두고 의존하는 쪽은 이 라이브러리만 바라보게 했다. 실행 대상이 아니라 `bootJar`는 끄고 일반 `jar`만 만들었고, `api(project(":common"))`로 공통 정의를 전이(transitive) 노출했다. 모듈 경계를 이렇게 잡아 두니 의존 관계가 빌드 설정만 봐도 드러났다.

&nbsp;&nbsp;&nbsp;프로젝트 셋업을 마치고 설계에서 도출한 이벤트와 Aggregate를 실제 코드로 옮기기 시작했다. 프레임워크는 [Axon](https://www.axoniq.io/)을 선택했다. 가장 큰 이유는 이벤트 소싱을 직접 구현하는 부담이었다. 이벤트 저장·재생, Command/Event/Query 분리, Projection 같은 플로우를 처음부터 직접 작성하는 것이 결국 생산성에 발목을 잡을것이라 생각했기 때문이다. 팀 전원이 Kotlin·Spring도 이벤트 소싱도 처음이었던 만큼, 러닝커브가 있더라도 검증된 프레임워크를 공부하며 빠르게 적용해보는 편이 낫다고 판단했다.

&nbsp;&nbsp;&nbsp;실제로 Axon은 필요한 기능을 폭넓게 제공했다. Aggregate·Command/Event/Query 버스·Event Store는 물론이고, 이벤트를 다시 흘려 Projection을 재구성하는 리플레이, 교차 Aggregate를 조정하는 Saga, 실패 이벤트를 격리하는 DLQ, AxonIQ Console 모니터링까지 운영 기능 상당수가 프레임워크 안에 이미 갖춰져 있었다.

&nbsp;&nbsp;&nbsp;전체 흐름을 먼저 그리면 다음과 같다.

```
[Aggregate] --(Event)--> [Event Store] --> [Axon Server]
                    │
                    └--(Projection)--> [Read Model]
```

&nbsp;&nbsp;&nbsp;쓰기(Command)는 Aggregate가 이벤트를 만들어 Event Store에 append하고, 읽기(Query)는 그 이벤트를 구독하는 Projection이 Read Model을 갱신하는 구조다. Bounded Context는 앞서 설계에서 정한 대로 배포 모듈 단위로 나눴고, 각 모듈 안에 도메인별 Aggregate가 자리 잡았다.

### 레거시와의 공존

&nbsp;&nbsp;&nbsp;가장 현실적인 제약은 레거시 Django를 당장 내릴 수 없다는 점이었다. 그래서 마이그레이션 기간 동안 두 시스템이 동시에 돌아가는 구조를 전제로 잡았다. 레거시에서 발생한 도메인 변경을 Kafka로 흘려보내면, 새 시스템의 Consumer가 이를 받아 Command로 변환해 Aggregate에 반영한다.

&nbsp;&nbsp;&nbsp;여기서 세 가지를 고민해야 했다.

1. 발행 — 정석대로라면 Outbox 테이블로 비즈니스 트랜잭션과 메시지 적재를 한 트랜잭션에 묶는 방법이 있다. 실제로 프로토타입까지 만들어 테스트해봤지만, 우리 규모에서는 과하다고 판단했다. 최종적으로는 Django signal로 도메인 변경 시점에 Kafka로 발행하는 단순한 방식을 택했다.
2. 정합성 검증 — signal 발행은 dual-write라 이론상 누락 창이 있다. 그래서 발행을 원천 차단하기보다 사후 대조로 보강했다. 레거시에 API relay를 두어, 해당 API가 호출되면 Celery task가 돌아 레거시 데이터와 새 시스템의 ES Projection을 비교·검증하고 불일치를 잡아냈다.
3. 기존 데이터 백필 — 실시간 변경분과 별개로, 이미 쌓여 있던 과거 데이터를 옮겨야 했다. 이를 위해 일반 Command와 구분되는 `Migrate*` 계열 커맨드/이벤트를 따로 두었다. 마이그레이션 이벤트는 검증 규칙이나 후처리를 실시간 이벤트와 다르게 태울 수 있어, 대량 적재와 실시간 처리를 깔끔하게 분리할 수 있었다.

### 시행착오

&nbsp;&nbsp;&nbsp;아무래도 제일 힘들었던 점은 초반 프로젝트 구조를 잡는 작업이었다. Django는 초기 프로젝트 셋업이 매우 간단한 편이다. 반면 Spring Boot는 config를 직접 작성해 의존성을 주입해줘야 했고, 이 과정에서 실수가 많이 발생했다(런타임에 설정 오류가 발목을 잡는 케이스가 잦았다). 추가적으로 JPA, Serializer 설정을 하는 데 꽤 애를 먹었는데, 자세한 내용은 다음 파트에서 다룬다.

&nbsp;&nbsp;&nbsp;그리고 코드가 Zero인 상태로 여럿이 함께 작업하다 보니 코드 일관성이 지켜지지 않았다. 프로젝트 초기에 AI 코드 지침 파일도 만들어 따르도록 강제했지만, 생각보다 버그도 많았고 제대로 따르지 않는 경우가 잦았다. 그래서 [konsist](https://github.com/LemonAppDev/konsist)를 도입해 코드 일관성을 강제했고(의존성 방향, 클래스명, 패턴 강제) 결과는 훨씬 성공적이었다. 어느 정도 코드 구조가 잡힌 이후로는 빠르게 마이그레이션을 진행할 수 있었다.

## TroubleShooting

---

&nbsp;&nbsp;&nbsp;이번 파트에서는 프로젝트를 진행하면서 겪은 TroubleShooting 들을 정리해보려 한다.

### Serializer

&nbsp;&nbsp;&nbsp;Axon의 기본 Serializer는 XStream이다. 하지만 XML 기반이라 가독성이 떨어지고, 범용성과 호환성을 생각해 Jackson serializer로 바꿨는데, 정작 Kotlin + Jackson + Axon 조합에서 예상치 못한 직렬화 오류가 발생하였다.

**① Aggregate 역직렬화 실패 — Kotlin backing field 접근 문제**

&nbsp;&nbsp;&nbsp;가장 먼저 막힌 지점이다. Axon이 저장된 이벤트로 Aggregate를 재구성할 때, Jackson이 Aggregate의 상태 필드를 읽어야 하는데 이게 계속 깨졌다. 원인은 Kotlin의 접근 제어였다. Aggregate 상태를 노출하는 getter를 `internal`로 선언해뒀는데, Jackson이 이 필드에 제대로 접근하지 못해 상태 복원이 실패한 것이다.

```kotlin
// Before — Jackson이 접근 못 함
internal fun getAccountInfo(): AccountInfo = accountInfo

// After — public으로 열어 해결
fun getAccountInfo(): AccountInfo = accountInfo
```

&nbsp;&nbsp;&nbsp;`internal`을 `public`으로 여는 단순한 수정이었지만, "직렬화기가 객체 내부를 어떻게 들여다보는가"를 이해하지 못하면 원인조차 못 찾는 종류의 버그였다.

**② List 필드의 null**

&nbsp;&nbsp;&nbsp;리스트 필드가 누락되거나 null로 들어올 때 역직렬화가 실패했다. 이벤트 스키마가 진화하면서 예전 이벤트엔 없던 필드가 생기는 경우가 있는데, 그때마다 터졌다. 리스트 계열 필드를 전부 `emptyList()`로 기본 초기화해 "없으면 빈 리스트"로 통일했다.

**③ 단일 값 vs 배열**

&nbsp;&nbsp;&nbsp;레거시가 보내는 JSON이 어떤 필드를 배열이 아니라 단일 값으로 담아 보내는 경우가 있었다. `ACCEPT_SINGLE_VALUE_AS_ARRAY` 옵션을 켜서, 단일 값이 와도 원소 하나짜리 배열로 받아들이게 했다.

**④ 커스텀 Deserializer**

&nbsp;&nbsp;&nbsp;레거시의 시간 표현이나 특정 타입은 표준 방식으로 안 읽혀서, `OffsetDateTimeDeserializer`·`PayTypeDeserializer` 같은 전용 역직렬화기를 만들어 레거시 포맷을 새 모델로 받아냈다. (시간 처리는 뒤의 타임존 이슈와도 이어진다.)

**그리고 진짜 문제 — 제각각인 ObjectMapper**

&nbsp;&nbsp;&nbsp;위 문제들을 하나씩 잡다 보니 더 근본적인 원인이 보였다. 여기저기서 `ObjectMapper`를 그때그때 새로 만들어 쓰고 있었던 것이다. 설정이 인스턴스마다 달라서, 어떤 경로에선 정상 동작하고 어떤 경로에선 깨졌다. 결국 설정을 한곳으로 모아 singleton으로 정리했다(`WRITE_DATES_AS_TIMESTAMPS` off, `FAIL_ON_UNKNOWN_PROPERTIES` off, `ACCEPT_SINGLE_VALUE_AS_ARRAY` on 등). 직렬화 설정은 흩어지는 순간 지옥이 된다는 걸 배웠다.

### Bulk 처리 30초 — 직렬 I/O 병목과 병렬 dispatch

&nbsp;&nbsp;&nbsp;운영에서 마주친 성능 문제다. 대량 적립 메시지 한 건에 약 1,500개의 거래가 담겨 발행됐는데, 이 메시지 하나를 consume하는 데 길게는 30초가 걸렸다. 처음엔 "발행 측에서 bulk 개수를 줄이자"는 생각이 먼저 들었지만, 그건 답이 아니었다. 총 작업량(1,500건 처리)은 그대로이기 때문이다.

&nbsp;&nbsp;&nbsp;원인을 파보니 CPU가 바쁜 게 아니었다. Axon의 `commandGateway.sendAndWait()`은 **블로킹** 호출이다. 커맨드를 보내고 Aggregate 로드 → 이벤트 저장 → 응답까지 그 스레드가 멈춰 기다리는데, 1건당 약 15\~20ms. 그런데 이걸 컨슈머 스레드 하나가 1,500번을 순차로 돌고 있었다.

```
1,500건 × 약 20ms ≈ 30,000ms = 30초
```

&nbsp;&nbsp;&nbsp;전형적인 직렬 I/O 병목이었다. 대부분의 시간이 gRPC 응답을 기다리는 대기 시간이라, 병렬로 처리하면 크게 줄일 수 있는 종류의 문제였다. 다만 아무렇게나 병렬화할 순 없었다. 적립 잔액은 유저별 누적값이라 **같은 유저의 거래는 순서를 지켜야** 정합성이 맞는다. 반면 서로 다른 유저는 완전히 독립적이다. 그래서 원칙을 "그룹 간 병렬 / 그룹 내 순차"로 잡았다 — 유저별로 묶어 그룹끼리는 병렬로, 한 그룹 안에서는 순차로.

```kotlin
val semaphore = Semaphore(bulkDispatchConcurrency)

val futures = transactions
    .groupBy { it.userId }                    // ① 유저별로 묶고
    .map { (_, userTransactions) ->
        CompletableFuture.runAsync({          // ② 그룹마다 가상 스레드로 병렬
            semaphore.acquire()               // ③ 동시 실행(=커넥션) 수 제한
            try {
                userTransactions.forEach { ... }  // ④ 같은 유저는 순차
            } finally {
                semaphore.release()
            }
        }, taskExecutor)
    }
CompletableFuture.allOf(*futures.toTypedArray()).join()   // ⑤ 전부 끝날 때까지 대기
```

&nbsp;&nbsp;&nbsp;여기서 몇 가지 함정을 만났다.

&nbsp;&nbsp;&nbsp;**세마포어로 동시성을 제한한 이유.** 가상 스레드는 1,500개를 만들어도 부담이 없다. 진짜 한정 자원은 스레드가 아니라 **DB 커넥션**이었다. `@Transactional`은 `sendAndWait`가 gRPC 응답을 기다리는 동안에도 커넥션을 놓지 않는다. 제한 없이 병렬화하면 bulk 하나가 커넥션 풀을 다 빨아들여, 다른 컨슈머나 Projection이 커넥션을 못 얻고 굶는다. 그래서 세마포어로 "동시에 일하는 개수 = 동시에 쥐는 커넥션 수"를 묶었다.

&nbsp;&nbsp;&nbsp;**세마포어 acquire 위치 버그.** 처음엔 `semaphore.acquire()`를 try 블록 밖에서 호출했는데, 인터럽트 등으로 acquire가 실패하는 경로에서도 finally의 release가 돌아 **획득하지도 않은 permit을 반납**하는 문제가 있었다. acquire를 try 안으로 옮겨 짝을 맞췄다.

&nbsp;&nbsp;&nbsp;**작업 완료 전에 리턴하면 안 된다.** 핸들러가 리턴하는 순간 Kafka offset이 커밋된다. `allOf().join()`으로 모든 워커가 끝날 때까지 기다리지 않으면, 아직 처리 중인 메시지가 "성공"으로 커밋되어 유실될 수 있다.

&nbsp;&nbsp;&nbsp;결과적으로 30초가 concurrency 8 기준 약 4초로 줄었다. 아키텍처나 이벤트 스키마는 건드리지 않고, 순서와 정합성도 보존한 채였다.

&nbsp;&nbsp;&nbsp;여기서 한 걸음 더 들어가면 근본 원인은 "`@Transactional`이 gRPC를 기다리는 내내 커넥션을 쥐고 논다"는 데 있었다. 커맨드 dispatch 경로는 사실 Axon Server(이벤트 저장소)로 가는 호출이라 그 구간에 DB 트랜잭션이 필요 없었다. 그래서 후속으로 이 경로의 트랜잭션 경계를 걷어내(`NOT_SUPPORTED`) dispatch 중 커넥션 점유를 0으로 만들었고, 그제서야 "커넥션 풀이 병렬도의 상한"이라는 제약 자체가 사라졌다. 성능 문제의 진짜 해결은 병렬화 자체보다 이 **커넥션 점유 시간을 줄인 것**이었다.

### JPA와 Bean 스캔 — 흩어진 설정을 끌어모으기

&nbsp;&nbsp;&nbsp;멀티모듈 + 헥사고날 구조로 나누다 보니, 정작 실행 모듈이 필요로 하는 빈·엔티티·설정이 여러 모듈과 패키지에 흩어졌다. Spring Boot는 기본적으로 `@SpringBootApplication`이 붙은 클래스의 하위 패키지만 자동 스캔한다. 그런데 실제로 필요한 것들은 그 바깥에 있었다.

- `common` 모듈의 공통 설정(`configs`, `axon`, `kafka`, `db` 등)
- `-api` 모듈에 정의한 JPA 엔티티
- Axon 프레임워크가 관리하는 엔티티(토큰 스토어, DLQ, Saga, 이벤트 스토어)

&nbsp;&nbsp;&nbsp;그래서 `@ComponentScan`과 `@EntityScan`의 대상 패키지를 명시적으로 넓혀줘야 했다.

```kotlin
@SpringBootApplication
@ComponentScan(basePackages = [
    "<도메인>", "axon", "configs", "kafka", "db",       // 공통 모듈의 빈·설정
    "org.axonframework.eventhandling.deadletter.jpa",  // Axon DLQ 빈
])
@EntityScan(basePackages = [
    "com.paywatch.<도메인>.api.outbound.db",             // Read Model 엔티티
    "org.axonframework.eventhandling.tokenstore.jpa",   // 토큰 스토어
    "org.axonframework.eventhandling.deadletter.jpa",   // DLQ
    "org.axonframework.eventsourcing.eventstore.jpa",   // 이벤트 스토어
])
class Application
```

&nbsp;&nbsp;&nbsp;이런 스캔 누락은 앞의 직렬화 오류와 성격이 다르다. 스캔에서 빠지면 필요한 빈을 못 찾아 **애플리케이션 기동 자체가 실패**한다. 실행되기 전에 걸리니 운영까지 새어 나가지는 않지만, "왜 로컬에선 되고 여기선 안 뜨지?" 하며 원인을 찾는 데 시간을 꽤 썼다. 결국 이 시기의 트러블슈팅은 대부분 **"흩어진 조각을 어디서 어떻게 끌어모으는가"** 라는 한 가지 주제로 수렴했다. 멀티모듈로 경계를 나눈 대가이기도 했다.

## 마치며

---

&nbsp;&nbsp;&nbsp;