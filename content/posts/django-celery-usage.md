---
title: "django 에서 celery 활용기"
subtitle: ""
date: 2022-04-19
description:
  - "실제 프로젝트에서 사용한 경험 공유"
  - "trouble shooting"
image: images/django-celery-usage/python-celery.png
meta_image: static/images/django-celery-usage/python-celery.png
tags:
  - django
  - celery
  - 비동기
  - asnyc
  - 분산락
  - distributed lock
categories:
  - django
---

## 들어가며

---

&nbsp;&nbsp;&nbsp;django로 서버를 운영하다보면 비동기 처리가 필요한 경우가 많다. 예를 들어 외부 API 호출, 데이터베이스 조회, 복잡한 계산 등등 많은 경우에 비동기 처리가 필요하다. 이때 celery를 활용하여 비동기 처리를 할 수 있다. celery에 관한 기본적인 설정이나 사용법들은 다른 블로그에 잘 나와있어서 이 포스트에서는 실제 프로젝트에서 사용한 경험과 유용했던 설정, 그리고 trouble shooting을 정리해보려 한다. (Broker 서버는 redis를 사용)


## celery 구성

---

&nbsp;&nbsp;&nbsp;아래는 celery를 실제 서비스에 사용하면서 효과를 보았던 설정들이다.
##### 1. 큐 분리


## djang-celery-beat

---

&nbsp;&nbsp;&nbsp;


## single task 처리

---

&nbsp;&nbsp;&nbsp;


## trouble shooting

---

&nbsp;&nbsp;&nbsp;