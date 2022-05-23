---
title: "[TIL] 쏘카 X 멋쟁이사자처럼 (AI 엔지니어 육성 부트 캠프 2기) 2주차"
subtitle: "Supervised Learning, Linear Model, Decision Tree, Ensemble"
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

## 2주차
---

1. Supervised Learning
2. Linear Model
3. Decision Tree
4. Ensemble
5. (실습) Logistic Regression
6. (실습) Decision Tree

### Supervised Learning
---

- 정답 레이블이 있는 training set (y=f(x))
- **Supervised Learning에 예시들:**
  - Linear Model: 가장 기본이 되는 모델
  - Linear Regression
  - Logistic Regression: decision boundary를 찾는것 
  - Support Vector Machine(SVM): Maximum margin seperator를 찾는 것 
  - Naive Bayes Classification: Bayes rule 확률 공식 사용, 가볍게 빠르게 작동하는(spam filter, recommend system)
  - Gaussain Process
  - K-Nearest Neighbors(KNN): Nonparametric approach(트레이닝 데이터가 늘어나면 parameters도 늘어남) -> Curse of dimensionality: 저차원이고 학습데이터가 많을때 작동
  - Decision Tree: Explainalbe, 간단하지만 성능이 좋음
  - Random Forest: Decision Tree의 Ensemble -> Variance 감소
  - Neural Network: Explainable 하지는 않음. gradient descent, input 데이터가 고차원인 경우에도 효과적

### Linear Model
---

- 간단한 모델 -> Generalization 
- 확장성: 다른 복잡한 모델의 기본이 되는 모델
- **Linear Regression**
  - y = wx + b
  - loss(오차) function MSE(제곱의 평균)
  - loss를 최소화 하기위한 w, b를 찾는 경사하강법(Gradient Descent)
  - 2차 함수의 기울기가 0이 되는것을 찾는것이 목적
  >
  {{< img src="/images/linear_regression.png" caption="Linear Regression" position="center" >}}   
  - closed-from solution, gradient descent
  - general linear model
- **Linear Classification**
   - linear decision boundary를 찾는 것이 목표
   - threshold function을 사용한 분류(0 or 1)
   - Perceptron(신경망)
- **Logistic Regression**
   - soft threshold, sigmoid 라고도 불림 (non linearlity)
   - MSE loss 대신 log loss 사용 (non linearlity 때문에)

### Decision Tree
---

- 예측변수(Predictor): input feature
- 예측변수의 공간을 여러영역으로 계층화, 분할
- Explainalbe 하다, 시각화하기 좋다
- over fitting에 취약, input data의 작은 변화에도 예측값이 크게 변화 -> Ensemble 기법
- **Regression Tree**
  - 전체 예측변수 공간을 겹치지 않는영역으로 분할한다(box)
  - 그곳에 속하는 training data 의 평균을 통해 예측값을 반환한다
  - Sum of squared errors (SSE)를 최소화하는 것이 목표
  - top-down, greedy: root 노드부터 SSE가 최소화 (Recursive Binary Splitting)
  - stopping criterion: over fitting을 방지하기 위해 leaf node의 데이터 수를 제한 하는것
  - Pruning a Tree (큰 Tree를 만든후 Pruning)
    - Cost complexity pruning(weakest link pruning)
- **Classification Tree**
  - 가장 많이 등장하는 class가 예측 class가 된다.
  - Classification error rate: 가장 비율이 높은 클래스의 비율만 고려
  - Gini index, Entropy: 한 클래스로 분포가 치우치게 되면 작은값, 골고루 분포되면 큰값 (node impurity) 
  - Pruning에서는 최종예측 정확도를 위해 Classification error rate를 주로 사용

### Ensemble Methods
---
- 여러 개의 간단한 'building block' 모델들을 결합해 하나의 강력한 모델을 만드는 법
- Decision Tree -> Bagging, Random Forest, Boosting
- **Bagging**
  - **B**ootstrap **ag**gregation
  - 다른 tree 여러개의 결과를 평균해서 최종 예측 -> **low variance**
  - Bootstrapping: 데이터셋에서 random한 복원추출을 통해 B개의 bootstrapped 데이터셋을 만드는 것
  - Out-of-Bag error estimation: bootstrapped 되지 않은 데이터를 validation error를 계산
  - Variable Importance Meassures: 모든 tree에 대한 예측변수의 splite으로 인해 SSE 감소한 정도를 확인
  - **Random Forests**
    - tree들을 decorrelate(data set들간의 correlation을 줄이는 것) 해주고자 split을 진행할 때마다 전체 p개의 예측변수 중 랜덤하게 m개의 변수를 뽑고, 이들만 고려하여 split 진행
- **Boosting** 
  - 학습한 Tree들의 정보를 이용하여 순차적으로 Tree 학습
  - 전체 data set을 사용, 잘못 예측한 데이터에 집중하여 반복학습을 시킨다.
  - 3개의 hyperparameter
    - Tree의 개수 B
    - Shirinkage parameter: 학습 속도를 조절, 0.01 ~ 0.001
    - split 횟수 d: boosted tree의 complexity를 조절, boosting을 통해 bias를 줄이므로 d가 클 필요가 없다.
    - Gradient Boosting, AdaBoost
