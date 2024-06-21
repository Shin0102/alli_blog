---
title: "AWS Lambda with SAM(작성중)"
subtitle: "AWS SAM으로 서버리스 애플리케이션 관리하기"
date: 2022-10-08
description:
  - "AWS LAMBDA"
  - "AWS SAM"
image: images/aws-lambda-with-sam/aws-lambda.png
meta_image: static/images/aws-lambda-with-sam/aws-lambda.png
tags:
  - aws lambda
  - aws sam
  - lambda environments
  - iac
  - serverless
categories:
  - infra
---

## 들어가며

---

&nbsp;&nbsp;&nbsp;서비스 개발을 하다보면 Serverless로 빠르게 개발이 필요한 경우가 생긴다. AWS를 쓰고있다면 Lambda 를 활용하면 되는데, 간단한 경우에는 콘솔로 코드를 직접작성하거나 코드를 직접 업로드 해도 되지만 결국 CI/CD 구성이 요구된다. Severless 배포 및 개발을 위한 도구들은 이미 많이 존재한다(Serverless, AWS SAM, Zappa(Python), Chalice(Python)) 그 중에서도 AWS SAM을 활용하여 AWS Lambda의 배포를 관리하는 방법을 정리해보려 한다. 단순히 CI/CD 뿐만 아니라 로컬 테스팅, IAC 의 효과까지 얻을수 있기 때문이다. 이번글을 통해서 SAM을 활용하여 AWS Lambda를 관리한 방법과 Trouble Shooting을 간단히 정리해보려 한다.

## AWS SAM 구조

---

&nbsp;&nbsp;&nbsp;AWS SAM은 AWS 종속적이긴 하지만, 현재 회사에서는 AWS Cloud를 적극적으로 사용하고 있고, 위에 언급한대로 local testing과 인프라관리까지 관리할수 있기 때문에(AWS CloudFormation 리소스도 포함가능하다), AWS SAM을 채택하였다. 세팅이나 구성은 [aws 공식문서](https://docs.aws.amazon.com/ko_kr/serverless-application-model/latest/developerguide/serverless-getting-started.html)를 참고하면 된다. 현재 사용중인 SAM 구조를 간소화해서 나타내면 아래와 같다.
```sh
$ tree

├── README.md
├── __init__.py
├── common_layer
│   ├── Makefile
│   ├── python
│   └── requirements.txt
├── events
│   └── event.json
├── lambda_function1
│   ├── __init__.py
│   ├── main.py
│   ├── src
│   └── test_main.py
├── lambda_function2
│   ├── __init__.py
│   ├── main.py
│   ├── src
│   └── test_main.py
├── mypy.ini
├── poetry.lock
├── pyproject.toml
├── samconfig.toml
├── template.yaml
└── tests
```
&nbsp;&nbsp;&nbsp;주요 파일들을 정리하면 아래와 같다.
- **common_layer/**: Lambda Layer를 구축하기 위한 Makefile 및 python dependency
- **lambda_function1/, lambda_function2/**: Lambda 소스
- **samconfig.toml**: 배포시 참조할 파라미터를 정의
- **template.yaml**: SAM 배포하기 위한 template을 정의
&nbsp;&nbsp;&nbsp;common_layer와 lambda_function 폴더는 예제설명을 위해 직접 추가한 폴더이며, samconfig 와 template 파일은 sam application 초기화시 자동으로 생성된다.

## 환경관리(Prod, Dev)

---

&nbsp;&nbsp;&nbsp;samcofing.toml의 파라미터 값을 template.yaml을 활용하면 배포환경을 분리하여 배포, 관리할수 있다. 아래 간단한 예제로 살펴보자.
```toml
# samconfig.toml
```
```yaml
# template.yaml
```


## Layer 관리

---

&nbsp;&nbsp;&nbsp;


## Local test

---

&nbsp;&nbsp;&nbsp;




## Troubleshooting

---

### 1. 

&nbsp;&nbsp;&nbsp;

### 2. 

&nbsp;&nbsp;&nbsp;
