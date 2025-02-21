---
title: "MSA 설계시 고려해야될 점과 Pattern"
subtitle: "MSA 설계시 고려해야될 점과 Pattern 요약"
date: 2021-06-01
description: 
- "MSA 설계시 고려해야될 점과 Pattern 요약"
image: images/msa.png
meta_image: static/images/msa.png
tags:
- MSA
categories:
- devops
---
## 고려해야 될점
---
- **Service Discovery** : Container가 Scailing 되어도 해당서비스를 찾을 수 있어야 함
    - 서비스 레지스트리 (Eureka, Consul) 활용
    - DNS 기반 서비스 디스커버리 (AWS Route53)
    - 쿠버네티스의 경우 Service, Ingress 리소스 활용

- **API Gateway** : 외부에서 내부의 서비스들에 접근에 대한 제어(인증, 로깅, 모니터링, Response)
    - 인증/인가 (OAuth2, JWT)
    - 속도 제한 (Rate Limiting)
    - 요청/응답 변환
    - 캐싱 전략
    
- **Load balance** : Container나 Instance 성능을 위해 외부요청을 적절히 분배
    - 로드밸런싱 알고리즘 (Round Robin, Least Connection 등)
    - Health Check 전략
    - Auto Scaling 정책

- **Circuit Breaker** : 하나의 서비스 장애가 전체 서비스의 장애가 되지 않아야함
    - Fallback 메커니즘
    - Timeout 설정
    - Retry 정책
    - Hystrix, Resilience4j 등 라이브러리 활용

- **IPC** : 서비스 사이의 통신
    - Sync : 
        - HTTP/REST: OpenAPI(Swagger) 명세
        - gRPC: 높은 성능, Protocol Buffers
    - Async : 
        - Message Queue 선택 기준 (처리량, 지연시간, 내구성)
        - Event-Driven Architecture 구현
        - Dead Letter Queue 처리

- **DDD** (Domain Drive Design -> Event storming, Bounded context) : MSA를 어떻게 구성할지에 대한 방법론
    - 전략적 설계 (Strategic Design)
    - 전술적 설계 (Tactical Design)
    - 컨텍스트 매핑 패턴

## MSA Pattern
---
- **SAGA** : MSA에서의 트랜잭션 처리
    - Choreography-Based Saga
        - 장점: 느슨한 결합, 간단한 구현
        - 단점: 순환 종속성 위험, 테스트 어려움
    - Orchestrations-Based Saga
        - 장점: 중앙 집중식 관리, 명확한 흐름
        - 단점: 오케스트레이터 복잡도 증가

- **Event Sourcing** : 발생하는 모든 event를 event table의 저장
    - 이벤트 저장소 구현 방법
    - 스냅샷 전략
    - 이벤트 버저닝
    - 동시성 처리

- **CQRS** : CUD와 Read를 분리(AWS DynamoDB와 같은 DB와 sync), Event Source 패턴과 결합
    - Read/Write 모델 분리 전략
    - 동기화 전략
    - 정합성 관리
    - 성능 최적화 방안

## 추가 고려사항
---
- **모니터링과 로깅**
    - 분산 추적 (Distributed Tracing)
    - 중앙 집중식 로깅
    - 메트릭 수집 및 알림

- **데이터 관리**
    - 데이터베이스 per 서비스
    - 데이터 일관성 전략
    - 마이그레이션 전략

- **보안**
    - 서비스간 인증/인가
    - 암호화 전략
    - 취약점 관리

- **배포 전략**
    - Blue/Green 배포
    - Canary 배포
    - Rolling 업데이트
