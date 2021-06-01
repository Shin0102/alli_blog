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
- **API Gateway** : 외부에서 내부의 서비스들에 접근에 대한 제어(인증, 로깅, 모니터링, Response)
- **Load balance** : Container나 Instance 성능을 위해 외부요청을 적절히 분배
- **Cirecuit Breaker** : 하나의 서비스 장애가 전체 서비스의 장애가 되지 않아야함
- **IPC** : 서비스 사이의 통신
    - Sync : HTTP, gRPC 
    - Async : RabbitMQ, AWS SQS, Kafka, Kinesis 등등
- **DDD** (Domain Drive Design -> Event storming, Bounded context) : MSA를 어떻게 구성할지에 대한 방법론


## MSA Pattern
---
- **SAGA** : MSA에서의 트랜잭션 처리
    - Choreography-Based Saga  
    - Orchestrations-Based Saga
- **Event Source** : 발생하는 모든 event를 event table의 저장
- **CQRS** : CUD와 Read를 분리(AWS DynamoDB와 같은 DB와 sync), Event Source 패턴과 결합
