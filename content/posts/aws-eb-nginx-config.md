---
title: "AWS Elastic Beanstalk 환경 구성하기"
subtitle: "AWS Elastic Beanstalk 환경 구성하기"
date: 2023-07-06
description:
  - ""
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

## EB 구성

---

{{< img src="images/aws-eb-nginx-config/aws_eb.png" title="aws eb 서버환경" caption="https://docs.aws.amazon.com/ko_kr/elasticbeanstalk/latest/dg/concepts-webserver.html" position="center" width="500px" >}}
&nbsp;&nbsp;&nbsp;먼저 간단히 AWS EB의 서버환경을 살펴보면 다음과 같다. 
- Elastic Load Balancer
- Auto Scaling Group
- EC2
- HM(HostManager): 각 EC2에 존재하며 EB의 배포,모니터링,로깅등을 담당한다.

&nbsp;&nbsp;&nbsp; 위 그림의 EC2 리소스에 애플리케이션 코드가 배포되며 EB는 Python, Java, Php등 다양한 언어의 플랫폼을 제공한다. 물론 docker를 이용한 배포 또한 지원하며, docker-compose를 이용하거나 Dockerrun.aws.json 파일을 통해 원격에 push된 이미지를 배포할수도 있다. 자세한 구성방법은 [AWS 공식 documentation](https://docs.aws.amazon.com/ko_kr/elasticbeanstalk/latest/dg/create_deploy_docker.html)을 참고하면 된다.

## EB 환경관리

---

&nbsp;&nbsp;&nbsp;
