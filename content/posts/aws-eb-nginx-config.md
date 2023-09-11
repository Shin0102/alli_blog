---
title: "AWS Elastic Beanstalk 환경 구성하기"
subtitle: "AWS Elastic Beanstalk 환경 구성하기"
date: 2023-07-06
description:
  - "AWS Elastic Beanstalk"
  - "EnvironmentVariables default value length is greater than 4096"
image: images/aws-eb-nginx-config/elastic_beanstalk_logo.png
meta_image: static/images/aws-eb-nginx-config/elastic_beanstalk_logo.png
tags:
  - aws
  - elastic beanstalk
  - nginx
  - infra
categories:
  - infra
---

## 들어가며

---

&nbsp;&nbsp;&nbsp;**Elastic Beanstalk**(이하 EB)은 어플리케이션 배포/관리(프로비저닝, 로드 밸런싱, 스케일링, 모니터링등)를 손쉽게 할수 있도록 도와주는 AWS 서비스중 하나이다. AWS에는 EB 이외에도 어플리케이션 배포를 위한 서비스들이 여러가지 존재한다. EC2, ECS, EKS, Serverless를 위한 lambda까지 당장 생각나는 것만 나열해도 꽤나 많다. 
&nbsp;&nbsp;&nbsp;현재 회사에서는 Docker 기반의 EB를 사용중이고 실제로 사용해보니 여러가지 장단점을 느낄수 있었다. EB 같은 경우 사용자가 어플리케이션을 배포하기 위한 복잡성을 최소화해주기 위해 제공하는 서비스이다 보니, 간단한? 설정만으로도 어플리케이션 운영을 위한 리소스들을 프로비저닝 해주고, 이를 관리하기 위한 리소스들 또한 손쉽게 설정이 가능하다. 
&nbsp;&nbsp;&nbsp;하지만 초기설정이 간단하다는 것은 추후 세부적인 커스텀을 위한 작업들이 불편하다는 것을 암시하기도 한다. 이 글을 쓰게 된것도 EB를 운영하면서 필요한 지식과 겪었던 트러블슈팅을 기록하기 위함이다.

## EB 구조

---

{{< img src="images/aws-eb-nginx-config/aws_eb.png" title="aws eb 서버환경" caption="https://docs.aws.amazon.com/ko_kr/elasticbeanstalk/latest/dg/concepts-webserver.html" position="center" width="500px" >}}
&nbsp;&nbsp;&nbsp;먼저 간단히 AWS EB의 서버환경을 살펴보면 다음과 같다. 
- Elastic Load Balancer
- Auto Scaling Group
- EC2
- HM(HostManager): 각 EC2에 존재하며 EB의 배포,모니터링,로깅등을 담당한다.

&nbsp;&nbsp;&nbsp; 위 그림의 EC2 인스턴스에 애플리케이션 코드가 배포되며 Python, Java, Php, Go등 다양한 언어의 플랫폼을 제공한다. 물론 docker를 이용한 배포 또한 지원하며, docker-compose를 이용하거나 Dockerrun.aws.json 파일을 통해 원격에 push된 이미지를 배포할수도 있다. 단일 컨테이너 환경을 구성할수도 있고 ECS cluster를 활용한 다중 컨테이너 배포 또한 가능하다. 인스턴스는 Auto Scaling Group(ASG)으로 scaling 할수 있으며, 자세한 구성방법은 [AWS 공식 documentation](https://docs.aws.amazon.com/ko_kr/elasticbeanstalk/latest/dg/create_deploy_docker.html)을 참고하면 된다.

## EB 구성

---

&nbsp;&nbsp;&nbsp;EB 구성은 가장 상위 개념의 Application이 있으며, 하위에 Environment를 생성할 수 있다. Environment 생성을 통하여 Application의 Prod, Stage, Dev등 다양한 환경을 구성할수 있으며, 각 Environment마다 EC2 인스턴스, 네트워크, 로드밸런서, 스케일링, 배포, 환경변수등 설정을 각각 관리할수 있다. 
- Application: Sample API Server

   - Prod Environment
   - Stage Environment
   - Dev Environment


## EB 환경

---

&nbsp;&nbsp;&nbsp;위에 언급한 EB 환경을 구성하는 방법에는 여러가지가 있는데, 
1. Console, EB CLI 등으로 직접설정하는 방법이 있고, 
2. 미리 저장된 구성을 로드하는 방법(save configuration), 
3. 구성 파일(.ebextensions)을 통한 설정방법, 
4. 설정하지 않은 값중 기본값이 존재하면 기본값으로 설정이 된다. 

&nbsp;&nbsp;&nbsp;만약 설정이 중복되는 경우 적용되는 우선순위는 나열한 순서대로이다. 구성 파일의 경우 소스 루트의 .ebextensions 폴더 하위에 .config 확장자로 파일을 생성하면 된다. 지원되는 format은 yaml, json이 있다. 

#### config example
```yml
option_settings:
  aws:elasticbeanstalk:environment:
    LoadBalancerType: network
```



## Troubleshooting

---

### 1. nginx config 세팅

&nbsp;&nbsp;&nbsp;eb 환경에서 reverse proxy를 설정할 수 있는데, 현재 nginx을 사용하고 있다. nginx를 사용하면 config를 수정해서 쓰게될 경우가 많은데, 이럴때 Configuration File을 통해 설정을 통해 수정이 가능하다. Amazon Linux 1 버전과 2 버전의 경로가 다르니 확인하여 적용해야 한다.
- Amazon Linux 1: .ebextensions/nginx
- Amazon Linux 2: .platform/nginx

#### config example: 
```sh
# .platform/nginx/conf.d/client_max_body_size.conf
client_max_body_size 20M;
```

### 2. EnvironmentVariables default value length is greater than 4096

&nbsp;&nbsp;&nbsp;갑자기 배포가 되지않고, 롤백되는 상황이 발생하여 event log를 확인해봤더니 위와 같은 로그를 확인하였다. 구글링을 해보니 [링크](https://stackoverflow.com/questions/54344236/environmentvariables-default-value-length-is-greater-than-4096)와 같은 이슈가 있었다. eb도 결국 cloudformation을 통하여 프로비져닝을 하기때문에 그에 따른 한계점이 존재하였다. 결론적으로는 환경 변수를 늘릴수 있는 방법은 없었고, 사용하지 않는 환경변수를 정리하거나, secret을 관리하는 다른 솔루션을 사용하는 방법밖에 없었다. 
