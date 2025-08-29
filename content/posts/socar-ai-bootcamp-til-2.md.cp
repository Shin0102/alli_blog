---
title: "Supervised Learning - Linear Model, Decision Tree, Ensemble"
subtitle: "[TIL] 쏘카 X 멋쟁이사자처럼 (AI 엔지니어 육성 부트 캠프 2기) 2주차"
date: 2022-05-18
description:
  - "Supervised Learning, Linear Model, Decision Tree, Ensemble"
image: images/socar-ai.png
meta_image: static/images/socar-ai.png
tags:
  - TIL
  - AI
  - Supervised Learning
  - Linear Model
  - Decision Tree
  - Ensemble
categories:
  - 쏘카 X 멋쟁이사자처럼 AI 부트 캠프
---

## 들어가며

---

&nbsp;&nbsp;&nbsp;2주차 강의에 접어들었다. 이번 강의는 Machine Learning중 output이 주어지는 **Supervised Learning**에 대해서, 그리고 기본이 되는 Linear Model에 대한 내용을 기반으로 Decision Tree, Ensemble 기법에 대한 내용 이었다. 이제 본격적으로 강의가 시작된 것인데, 엄청 깊게 들어가지는 않지만 아직 용어나 개념에 익숙치 않다보니 쉽지 않은 내용이었다. 잡설은 그만하고 강의 내용 정리해보자.
{{< alert theme="success" >}}
깊이보다는 AI에 대한 전반적인 지식을 기록한 글입니다.
{{< /alert >}}

## 2주차

---

1. Supervised Learning
2. Linear Model
3. Decision Tree
4. Ensemble
5. (실습) Logistic Regression
6. (실습) Decision Tree

## Supervised Learning

---

- 지도 학습, 정답 레이블이 있는 학습방법, input x에 대해서 output 을 예측하는 방법이다.
- Train 단계에서 모델을 fitting 하고, Test 단계에서 fitting 한 모델 inference(추론)를 진행한다.
  <br>
  > **y = f(x)** (input x에 대한 y값을 추론)

### Supervised Learning에 다양한 예시들

- Linear Model: 다른 복잡한 모델의 가장 기본이 되는 모델.

  - Linear Regression
  - Logistic Regression: decision boundary를 찾는 알고리즘, 데이터를 분류할때 사용한다.
  - Support Vector Machine(SVM): decision boundary(Maximum margin seperator)를 찾는 알고리즘

    - Maximum Margin Seperator: Seperator(선)를 기준으로 양옆으로 데이터 margin이 가장 먼 Seperator
    - Support Vector: Maximum Margin Seperator와 가장 접해있는 데이터
      <br>

- Naive Bayes Classification: 확률 기반 model, Bayes Rule 이라는 확률공식을 사용, 정확도 보다는 가볍고 빠르게 돌아야하는 어플리케이션에서 사용. (ex. spam 필터링, 문장 감정 분석, 추천 시스템)
- Gaussian Process: 확률 기반 model, 데이터 {x, f(x)}가 Multivariate(다변수) Gaussian 분포라 가정, 예측에 대한 confidence를 알 수 있음
- K-Nearest Neighbors(KNN):

  - Nonparametric approach (Training 데이터가 늘어나면 paramter 개수도 늘어나는 형태)
  - 트레이닝 데이터를 모두 저장해 놨다가, 새로운 input 데이터가 들어왔을때 가장 가까운 k개의 값들을 통해 output을 알아낸다.
  - Curse of dimensionality: input feature가 고차원일때 많은 양의 학습데이터 필요하다.
    <br>

- Decision Tree: Explainable 하다. 사람의 사고방식과 유사. Overfitting이 되기 쉽다.
- Random Forest: 여러개의 Decision Tree의 Ensemble, 사람들의 집단지성과 같은 느낌이다. Variance가 감소
- Neural Network
  <br>
  > **어떤 상황에도 잘맞는 알고리즘은 없다. 데이터의 형태나 개수 컴퓨팅 자원등을 고려하여 적절한 알고리즘을 사용하여야 한다.**

## Linear Model

---

- 다른 복잡한 모델의 기본이 되는 모델이다.
- 간단한 모델 -> Generalization
- 확장성: 다른 복잡한 모델의 기본이 되는 모델

### Linear Regression

- y = wx + b
- loss(오차)으로는 MSE(제곱의 평균) function을 사용
- loss를 최소화 하기위한 w, b를 찾는다.
- Gradient Descent(경사하강법): 2차 함수의 기울기가 0이 되는것을 찾는것이 목적
- closed-from solution, gradient descent
- general linear model: nonlinear 한 데이터를 fitting

### Linear Classification

- linear decision boundary를 찾는 것이 목표
- threshold function을 사용한 분류(0 or 1)
- Perceptron(신경망)

### Logistic Regression

- logistic 함수를 이용해서 class label이 1일 확률을 예측
- soft threshold, sigmoid 라고도 불림 (non linearlity)
- MSE loss 대신 log loss 사용

  ㄴ non linearlity 때문에

  ㄴ MSE를 사용하면 loss function이 2차함수의 형태가 아니게 되므로...

## Decision Tree

---

- 예측변수(Predictor): input feature
- 예측변수의 공간을 여러영역으로 계층화, 분할
- Explainalbe 하다, 시각화하기 좋다
- over fitting에 취약, input data의 작은 변화에도 예측값이 크게 변화 -> Ensemble 기법

### Regression Tree

- 전체 예측변수 공간을 겹치지 않는영역으로 box들로 분할한다
- 그곳에 속하는 training data 의 평균을 통해 예측값을 반환한다
- Sum of Squared Errors (SSE)를 최소화하는 것이 목표
- top-down, greedy: root 노드부터 단계별로 SSE가 최소화되는 split을 찾는다. (Recursive Binary Splitting)
- stopping criterion: over fitting을 방지하기 위해 leaf node의 데이터 수를 제한 하는것(ex. leaf node의 개수를 5개로 제한)
- Pruning a Tree (큰 Tree를 만든후 Pruning(가지치기))

  ㄴ Cost complexity pruning(weakest link pruning)

### Classification Tree

- 가장 많이 등장하는 class가 예측 class가 된다.
- Classification error rate: 가장 비율이 높은 클래스의 비율만 고려
- Gini index, Entropy: 한 클래스로 분포가 치우치게 되면 작은값, 골고루 분포되면 큰값 -> 작은값 일수록 좋다. (node impurity라고도 부른다)
- Pruning에서는 최종예측 정확도를 위해 Classification error rate를 주로 사용

## Ensemble Methods

---

- 여러 개의 간단한 '**building block**' 모델들을 결합해 하나의 강력한 모델을 만드는 법
- Decision Tree -> Bagging, Random Forest, Boosting

### Bagging

- **B**ootstrap **ag**gregation
- Variance 감소 효과
- 다른 tree 여러개의 결과를 평균해서 최종 예측 -> **low variance**
- Bootstrapping: 데이터셋에서 random한 복원추출을 통해 B개의 bootstrapped 데이터셋을 만드는 것
- Out-of-Bag error estimation: bootstrapped 되지 않은 데이터를 validation error를 계산
- Variable Importance Meassures: 모든 tree에 대한 예측변수의 split으로 인해 SSE 감소한 정도를 측정하여 평균을 취함. 예측 변수가 판단에 도움이 어느정도 되는지 파악하는데 도움이 된다.
- **Random Forest**
  - tree들을 decorrelate(data set들간의 correlation을 줄이는 것) 해주고자 split을 진행할 때마다 전체 p개의 예측변수 중 랜덤하게 m개의 변수를 뽑고, 이들만 고려하여 split 진행

### Boosting

- Bagging과 다르게 학습한 Tree들의 정보를 이용하여 순차적으로 Tree 학습
- Bias 감소 효과
- 전체 data set을 사용, 잘못 예측한 데이터에 집중하여 반복학습을 시킨다.
- 3개의 hyperparameter

  - Tree의 개수 B: 너무 크면 overfit 될 수 있다.
  - Shirinkage parameter: 학습 속도를 조절, 0.01 ~ 0.001
  - split 횟수 d: boosted tree의 complexity를 조절, boosting을 통해 bias를 줄이므로 d가 클 필요가 없다.
  - Gradient Boosting, AdaBoost
