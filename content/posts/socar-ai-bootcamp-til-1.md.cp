---
title: "AI, Machine Learning, Deep Learning?"
subtitle: "[TIL] 쏘카 X 멋쟁이사자처럼 (AI 엔지니어 육성 부트 캠프 2기) 1주차"
date: 2022-05-17
description:
  - "기본 개념 및 머신러닝의 이해"
image: images/socar-ai.png
meta_image: static/images/socar-ai.png
tags:
  - TIL
  - AI
  - Machine Learning
  - Deep Learning
categories:
  - 쏘카 X 멋쟁이사자처럼 AI 부트 캠프
---

## 들어가며

---

&nbsp;&nbsp;&nbsp;AI에 대해서 흥미는 있었지만, 막연하게 미디어에서만 접하다가 좀 더 깊이 있게 배워보고 싶다는 생각을 하던 중에 project lion이란 곳에서 AI 부트 캠프를 시작한다는 이야기를 듣고 덜컥 신청해버렸다. 이번년도에 뭔가 새로운 분야를 공부하면 좋겠다 생각을 했었는데, 좋은 기회가 될 것 같다. 총 8개의 챕터로 구성되어 있고 온라인으로 진행된다. 매주 한 챕터씩 강의가 오픈되며 중간중간 과제와 마지막 해커톤(5주)으로 총 13주로 마무리되는 일정이다. TIL을 통해 강의 내용을 정리하고 부족한 부분을 채워넣을 생각이다. 1주차는 AI의 전반적인 가벼운 소개로 시작되었고, 그 중 Machine Learning에 대해서 중점적으로 강의가 진행 되었다.

{{< alert theme="success" >}}
깊이보다는 AI에 대한 전반적인 지식을 기록한 글입니다.
{{< /alert >}}

## 1주차

---

1. AI, 머신러닝, 딥러닝의 이해
2. 머신러닝 basic
3. 머신러닝 문제의 분류
4. 학습방법의 분류
5. (실습) numpy

## AI > Machine Learning > Deep Learning

---

&nbsp;&nbsp;&nbsp;AI, ML, DL의 관계는 위와같이 포함관계라고 한다. 아직은 용어나 알고리즘 이름들이 익숙치 않지만 정리해보면 아래와 같다.

### 1. AI

- 사람처럼 행동하고 생각하고, 합리적으로 생각하고 행동한다. 이것을 컴퓨터로 구현한 기술
- 기존의 Traditional AI: Rule-based System, 분명한 한계점이 존재하여 Machine Learning의 등장하게 되었다.
- Traditional AI 종류에는 Search alg, Propositional Logic, First-Order Logic, Plannig 등이 있다.
  >

### 2. Machine Learning

- 주어진 데이터로부터 학습을 하고, 새로운 정보를 얻어내거나 예측하는 기술. 데이터가 많을수록 성능이 좋아진다.
- Supervised Learning, Unsupervised Learning
- ex. Linear regression, Decision tree, K-means Clustering
  >

### 3. Deep Learning

- **neural network**
- Hierarchial representation learning (계층적 학습)
- ex. visual / speech recognition 나 structured data에 활용(개인화 추천)

## Machine Learning

---

- ML의 프로세스는, Training 단계에서 training data를 통해서 모델을 학습하고, 학습한 모델을 통해서 Test 단계에서 test data를 통해 모델의 성능을 측정한다.

### ML Data set

- traing set(학습을 위한), validation set(모델을 선정하기 위한), test set(성능을 측정하기 위한)
- Data set이 충분히 크지 않다면? **k-fold cross validation**을 이용한다.
- **k-fold cross validation**: Data set을 K개의 부분집합으로 Data set을 구성하고 그 중 1개는 validation set 나머지는 training set으로 사용한다, 데이터수가 너무 적게되면 underfitting 되어 모델의 성능이 좋지 않을 수 있기때문에 사용한다.
  >

### ML의 목적

- 새로운 input data가 들어오더라도 좋은 성능을 내는 모델을 학습
- underfitting(high bias(편향)) -> optimization, more complex model 필요
- overfitting(high variance(분산)) -> regularization, more data 필요
- 위의 under, overfitting을 피하기 위해서 적절한 **bias-variance Trade off** 가 필요하다.
  {{< box >}}
  {{< img src="/images/bias-variance.png" caption="출처: http://scott.fortmann-roe.com" width="350px" position="center" >}}
  {{< /box >}}
  >

### ML 문제(task) 분류

- **Classifiation**: 데이터를 label하고 새로운 데이터가 들어왔을때 labeling하는 것(분류)
- **Regression**: input 데이터에 대한 output 데이터를 맵핑 ex. Linear regression, Logisitic regression
- **Densitiy Estimation**: input 데이터에 확률분포(패턴)을 찾는 것
  >

### ML 학습 방법의 분류

- **Supervised Learning:** input에 대한 output에 대한 값이 주어짐(labeled) ex. 알파고, siri, translator
- **Unsupervised Learning:** input에 대한 output이 아닌 의미있는 패턴을 찾는 것(unlabed) ex. Clustering, auto-encoder
- Reinforcement Learning: observation하고 action에 대해 reward값을 줌으로써 학습한다.
- Semi-supervised Learning
- Self-supervised Learning
