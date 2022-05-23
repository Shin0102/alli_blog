---
title: "[TIL] 쏘카 X 멋쟁이사자처럼 (AI 엔지니어 육성 부트 캠프 2기) 1주차"
subtitle: "기본 개념 및 머신러닝의 이해"
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

## 1주차
---

1. AI, 머신러닝, 딥러닝의 이해
2. 머신러닝 basic
3. 머신러닝 문제의 분류
4. 학습방법의 분류
5. (실습) numpy

### AI > Machine Learning > Deep Learning
---

**1. AI**
- 사람처럼 행동하고 생각하고, 합리적으로 생각하고 행동한다.
- Traditional AI: Rule-based System, 한계점이 존재 -> Machine Learning의 등장 
- Tradtional AI ex: search alg, propositional logic, first-order logic, planning
>
**2. Machine Learning**
- 데이터로부터 학습 (데이터가 많을수록 성능이 좋아짐)
- ex. Linear regression, Decision tree, K-means Clustering
>
**3. Deep Learning**
- Hierarchial representation learning (계층적 학습)
- visual / speech recognition 나 structured data에 활용(개인화 추천)

### Machine Learning 
---

- Training(training data) -> Test(test data) -> measure the performance
- **Data set:**
   - traing set, validation set(모델을 선정하기위한), test set(성능을 측정하기 위한)
   - Data set이 충분히 크지 않다면? k-fold cross validation
   - k-fold cross validation: Data set을 K개의 부분집합으로 Data set을 구성 -> 그 중 1개는 training 나머지는 training
>
- **ML의 목적:**
   - 새로운 input data가 들어오더라도 좋은 성능을 내는 모델을 학습
   - underfitting(high bias) -> optimization, more complex model
   - overfitting(high variance) -> regularization, more data
   - 적절한 bias-variance Trade off 가 필요함
   {{< box >}}
   {{< img src="/images/bias-variance.png" caption="출처 http://scott.fortmann-roe.com" width="350px" position="center" >}}
   {{< /box >}}
>
- **ML 문제(task)의 분류:**
   - Classifiation: 데이터를 label하고 새로운 데이터가 들어왔을때 label하는 것(분류)
   - Regression: input 데이터에 대한 output 데이터를 맵핑 ex. Linear regression, Logisitic regression
   - Densitiy Estimation: input 데이터에 확률분포(패턴)을 찾는 것
>
- **학습 방법의 분류:**
   - **Supervised Learning:** input에 대한 output에 대한 값이 주어짐(labeled) ex. 알파고, siri, translator
   - **Unsupervised Learning:** input에 대한 output이 아닌 의미있는 패턴을 찾는 것(unlabed) ex. Clustering, auto-encoder
   - Reinforcement Learning: observation하고 action에 대해 reward값을 알려줌으로써 학습
   - Semi-supervised Learning
   - Self-supervised Learning
