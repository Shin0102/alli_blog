---
title: "AWS Lambda with SAM"
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

&nbsp;&nbsp;&nbsp;서비스 개발을 하다보면 Serverless로 빠르게 개발이 필요한 경우가 생긴다. AWS를 쓰고있다면 Lambda 를 활용하면 되는데, 간단한 경우에는 콘솔로 코드를 직접작성하거나 코드를 직접 업로드 해도 되지만 결국 CI/CD 구성이 요구된다. Severless 배포 및 개발을 위한 도구들은 이미 많이 존재한다(Serverless, AWS SAM, Zappa(Python), Chalice(Python)) 그 중에서도 AWS SAM을 활용하여 AWS Lambda의 배포를 관리하는 방법을 정리해보려 한다. 단순히 CI/CD 뿐만 아니라 로컬 테스팅, IAC 의 효과까지 얻을수 있기 때문이다. 이번글을 통해서 SAM을 활용하여 AWS Lambda를 관리한 방법을 간단히 정리해보려 한다.

## AWS SAM 구조

---

&nbsp;&nbsp;&nbsp;AWS SAM은 AWS 종속적이긴 하지만, 현재 회사에서는 AWS Cloud를 적극적으로 사용하고 있고, 위에 언급한대로 local testing과 인프라 관리까지 관리할수 있기 때문에(AWS CloudFormation 리소스도 포함가능하다([링크](https://docs.aws.amazon.com/serverless-application-model/latest/developerguide/sam-specification-resources-and-properties.html) 참고), AWS SAM을 채택하였다. 세팅이나 구성은 [aws 공식문서](https://docs.aws.amazon.com/ko_kr/serverless-application-model/latest/developerguide/serverless-getting-started.html)를 참고하면 된다. 현재 사용중인 SAM 구조를 간소화해서 나타내면 아래와 같다.
```sh
$ tree

├── README.md
├── __init__.py
├── common_layer
│   ├── Makefile
│   ├── python
│   └── requirements.txt
├── events
│   └── event.json
├── lambda_function1
│   ├── __init__.py
│   ├── main.py
│   ├── src
│   └── test_main.py
├── lambda_function2
│   ├── __init__.py
│   ├── main.py
│   ├── src
│   └── test_main.py
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

&nbsp;&nbsp;&nbsp;samcofing.toml의 파라미터 값을 template.yaml에 활용하면 배포환경을 분리하여 배포, 관리할수 있다. 간단하게 Lambda function 2개를 API Gateway 1개로 서빙하는 구조를 예제로 만들어보면 아래와 같다.
```yaml
# template.yaml
AWSTemplateFormatVersion: '2010-09-09'
Transform: AWS::Serverless-2016-10-31
Description:
  '
  SAM example
  '

Parameters:
  Env:
    Type: String
    Description: Which environment do you want to deploy to? (dev, prod)
    AllowedValues:
      - dev
      - prod
    Default: dev
  SubnetA:
    Type: String
  SubnetB:
    Type: String


Resources:
  LambdaFunction1:
    Type: AWS::Serverless::Function
    Properties:
      FunctionName: !Sub "Lambda-${Env}-Lambda-Function-1"
      CodeUri: lambda_function1
      Handler: main.handler
      Runtime: python3.9
      Architectures:
      - x86_64
      Events:
        LambdaFunction1:
          Type: Api
          Properties:
            Path: /first/{proxy+}
            Method: ANY
            RestApiId: !Ref API
      VpcConfig:
        SubnetIds:
          - !Sub ${SubnetA}
          - !Sub ${SubnetB}
    Metadata:
      SamResourceId: LambdaFunction1

  LambdaFunction2:
    Type: AWS::Serverless::Function
    Properties:
      FunctionName: !Sub "Lambda-${Env}-Lambda-Function-2"
      CodeUri: lambda_function2
      Handler: main.handler
      Runtime: python3.9
      Architectures:
      - x86_64
      Events:
        LambdaFunction2:
          Type: Api
          Properties:
            Path: /second/{proxy+}
            Method: ANY
            RestApiId: !Ref API
      VpcConfig:
        SubnetIds:
          - !Sub ${SubnetA}
          - !Sub ${SubnetB}
        SecurityGroupIds:
          - !Sub ${securityGroupId}
    Metadata:
        SamResourceId: LambdaFunction2

  API:
    Type: AWS::Serverless::Api
    Properties:
      StageName: !Sub ${Env}
```
{{< boxmd >}}
- **line 9~20**   : template에서 사용할 parmeter 정의, samconfig.toml을 통해 값을 주입함.
- **line 24~45**  : lambda_function1 정의, parameter 값을 활용하여 Lambda 함수명과 subnet 환경분리.
- **line 33~39**  : lambda_function1 의 트리거 이벤트 정의, 다양한 리소스지정(S3, SNS Event...)이 가능하며, 예제에서는 API Gateway의 **'/first'** path를 통해 요청이 들어올경우 lambda_function1이 트리거 된다.
- **line 47~70**  : lambda_function2 정의, parameter 값을 활용하여 Lambda 함수명과 subnet 환경분리. API Gateway의 **'/second'** path를 통해 요청이 들어올경우 lambda_function2이 트리거 된다.
- **line 72~75**  : Lambda를 서빙하기위한 API GW 정의
{{< /boxmd >}}


```toml
# samconfig.toml
version = 0.1
[default]
[default.deploy]
[dev.deploy.parameters]
stack_name = "CF-DEV-SAM-EXAMPLE-APP"
region = "ap-northeast-2"
confirm_changeset = true
capabilities = "CAPABILITY_IAM"
parameter_overrides = """Env=\"stage\" \
SubnetA=\"사용중인 dev subnet id\" \
SubnetB=\"사용중인 dev subnet id\""""

[prod.deploy.parameters]
stack_name = "CF-PROD-SAM-EXAMPLE-APP"
region = "ap-northeast-2"
confirm_changeset = true
capabilities = "CAPABILITY_IAM"
parameter_overrides = """Env=\"prod\" \
SubnetA=\"사용중인 prod subnet id\" \
SubnetB=\"사용중인 prod subnet id\""""
```
{{< boxmd >}}
- **line 5~12** : SAM config dev deploy section, stack_name은 SAM 배포시 사용되는 Cloudformation명을 정의한것이고 parameter_overrides 옵션을 통해 SAM template에 전달할 parameter를 정의한다. 
- **line 14~21** : SAM config production deploy section
- 이렇게 정의한 deploy option은 배포시 ```--config-env``` 옵션으로 지정할수 있다.
- 배포 예시
  ```bash
  sam deploy --config-env dev   # dev
  sam deploy --config-env prod  # prod
  ```
{{< /boxmd >}}

## Layer 관리

---

&nbsp;&nbsp;&nbsp;Labmda의 경우 코드 업로드 사이즈 크기에 제한이 있기 때문에, 외부패키지나 공용 모듈의경우 Lambda Layer로 따로 관리하는게 좋다. SAM에서도 Lambda Layer를 bulid하여 배포가 가능하다. 자세한 방법은 [링크](https://docs.aws.amazon.com/serverless-application-model/latest/developerguide/building-layers.html)를 확인하면 되며, 최종적인 SAM template은 아래와 같다. 
```yaml
# template.yaml

# ...생략

Resources:
  # Layer 추가
  CommonLayer:
    Type: AWS::Serverless::LayerVersion
    Properties:
      ContentUri: common_layer
      CompatibleRuntimes:
        - python3.9
    Metadata:
      BuildMethod: makefile

  LambdaFunction1:
    Type: AWS::Serverless::Function
    Properties:
      FunctionName: !Sub "Lambda-${Env}-Lambda-Function-1"
      CodeUri: lambda_function1
      Handler: main.handler
      Runtime: python3.9
      Layers: 
        - !Ref CommonLayer  # Layer property 추가
  
# ... 생략
```

## Local test

---

&nbsp;&nbsp;&nbsp;SAM을 활용하여 local을 테스트하는 옵션은 여러가지 있는데, 주로 사용하는 옵션 두가지는 아래와 같다.
- `sam local start-api` : 로컬에서 API Gateway를 실행하는 옵션, 로컬 서버처럼 테스트 가능하다
- `sam local invoke` : 로컬에서 Lambda 함수를 실행하는 옵션, API Gateway가 아닌 다른 event로 실행되는 경우 활용한다.
  - 예를들어 AWS SNS의 notification이 발생되면 Lambda 함수가 실행되는 경우 테스트는 아래와 같다.
    ```bash
    sam local generate-event --help  # --help로 event list 확인 가능
    sam local generate-event sns notification > events/sns-event.json # sns-event.json 생성
    sam local invoke TestFunction -e events/sns-event.json # TestFunction 실행
    ```


## 마치며

---

&nbsp;&nbsp;&nbsp;현재 회사에서 SAM을 사용하여 10여개의 Lambda와 관련 리소스들을 관리하고 있다. 기존의 경우 Lambda의 형상관리가 되지 않거나 중구난방으로 배포되고 있었는데, 위와 같이 SAM으로 구성한 뒤로는 하나의 Repository 에서 Lambda를 관리하고 배포 자동화까지 구축하게 되었다. 또한 samconfig를 통해 환경구축을 쉽게 분리 할수있게 되어 테스트나 PoC를 위한 구축도 매우 빠르게 진행할 수 있게되었다.