---
title: "Backward Pass, Optimizer, Batch Normalization"
subtitle: "[TIL] 쏘카 X 멋쟁이사자처럼 (AI 엔지니어 육성 부트 캠프 2기) 5주차"
date: 2022-06-07
description:
  - "인공신경망, 어떻게 학습 시키는 거죠?"
image: images/socar-ai.png
meta_image: static/images/socar-ai.png
tags:
  - TIL
  - AI
  - Backward Pass
  - Optimizer
  - Batch Normalization
categories:
  - 쏘카 X 멋쟁이사자처럼 AI 부트 캠프
---

## 들어가며

---

&nbsp;&nbsp;&nbsp;5주차 강의에 접어들었다. 이번장에서는 MLP의 전반적인 과정에 대한 내용이었고, 그중에서 Forward Pass, Backward Pass에 대한 내용이 주요 내용이었다. 그리고 최적의 모델 성능을 위한 Optimizer 와, 학습속도와 안정도를 위해 사용하는 Batch Normalization 내용도 포함 되었다. 가장 멘붕오는 강의였던 것 같다. 같은 피어그룹 분들도 모두 동의하였다. 그래도 공유된 유튜브 강의나 블로그등을 보면서 조금이나마 이해도를 높이는데 도움이 된 것 같다.
{{< alert theme="success" >}}
깊이보다는 AI에 대한 전반적인 지식을 기록한 글입니다.
{{< /alert >}}

## 5주차

---

1. Backward Pass
2. Optimizer
3. 외우지 않고 배우는 모델
4. Batch Normalization
5. (실습) MLP MNIST classification (2)

## Backward Pass

---

### Back Propagation 이란?

- loss를 줄이는 방향으로 각 parameter를 조절하기위해 각 parameter에 대한 loss의 편미분 값을 계산하여 이를 이용해 parameter를 update
- 편미분(Partial Derivative): MLP와 같은 다변수 함수는 각 변수들이 복합적으로 함수에 영향을 주기 때문에 다른 변수들의 값을 상수로 둔 상태에서 특정 변수에 대한 도함수 값을 고려
- Chain Rule: 연쇄 법칙을 활용하여, Forward Pass 시 저장하였던 Local Gradient와 Back Propagation으로 역으로 내려온 Global Gradient 값을 이용하여 전체 Loss 에 편미분 값을 구할수 있다.

### Forward Pass - Backward Pass

- MLP의 연산들은 matrix multiplication과 nonlinear activation function 으로 구성되어 아래와 같이 구분가능
- Forward Pass 들의 basic operation 4가지를 보고, Backward Pass를 할때 에러 시그널이 어떻게 전달되는지 살펴보자.
  <br>
  {{< img src="/images/socar-ai-bootcamp-til-5/addition.PNG" title="1" caption="Addition Operation" position="center" width="350px" >}}
  {{< img src="/images/socar-ai-bootcamp-til-5/multiple.PNG" title="2" caption="Multiplication Operation" position="center" width="350px" >}}
  {{< img src="/images/socar-ai-bootcamp-til-5/common variable.PNG" title="3" caption="Common Variable Operation" position="center" width="350px" >}}
  {{< img src="/images/socar-ai-bootcamp-til-5/Nonlinear Active Fucnction.PNG" title="4" caption="Nonlinear Active Fucnction" position="center" width="350px" >}}
- Nonlinear Activation Function

  - Activation function의 backward pass를 위해서는 해당 node에서 각 함수의 미분값이 필요.

    - Sigmoid, ReLu ...

## Optimizer

---

- gradient descent를 통해서 loss에 대해서 최적 parameter 값을 구하기 위해서 여러번 업데이트를 진행한다.
- Deep Learning 에서는 parameter space는 차원이 굉장히 크다. -> global optimal point 찾는 것이 불가능.
- saddle point(말 안장의 모양)를 피하고 local minima를 찾는데 목표로 한다.

### Gradient-based Methods

- First-order Optimization Methods
- Parameter를 loss function gradient의 반대방향으로 update 하여 loss function이 더 작은 paramter를 얻음
- mini-batch를 이용

1. Stocahstic Gradient Descent SGD

   - Parameter를 gradient 반대 방향으로 update
   - 가장빠르고 쉽게 적용가능하나 saddle points에 빠지기 쉬움
   - Gradeint에 noise가 많이 발생, update의 방향이 진동하기 쉬움

2. Momentum

   - gradeint가 빠르게 변하는 것을 막으며 일관된 방향으로 update 유도
   - Hyper-parameter momentum factor가 추가 됨

3. AdaGrad

   - Update 방향이 과하게 진동하는 문제를 해결하기 위해 prameter-wise update
   - history를 통해 parameter-wise learning rate 적용
   - Update 양이 많은 parameter의 update를 줄이고, update가 많이 진행되지 않은 parameter의 update를 늘림
   - learning rate가 계속 감소하여, Deap Learning에서 사용하기 어렵다.

4. RMSprop

   - AdaGrad의 gradient accumulation S의 momentum을 적용
   - 너무 먼 과거의 gradient의 효과를 줄임

5. Adam

   - 가장 많이 사용됨
   - RMSprop과 momentum의 조합
   - Bias correction이라는 기법을 통해 각 momentum이 초반에 불안정하게 작동하는 것을 방지
     <br>

---

- Learning Rate Scheduling

  - 학습이 직행될수록 parameter가 최적 값으로 다가가기 때문에 learning rate를 줄여 더 정확한 수렴을 시도
  - Linear decay, step decay, exponential decay

- Parameter Initialization

  - 초기 parameter 설정도 중요하다.

## 외우지 않고 배우는 모델

---

- Regularization

  - Overfitting을 막기 위한 기법
  - Overfitting: 데이터의 존재하는 noise까지 학습함에 따라 학습 데이터가 아닌 데이터에 대해 정확한 추론을 하지 못하는 경우

  1. Norm Regularizations
  2. Early Stopping
     - Validation set의 성능향상이 더 이상 나타나지 않을때 학습을 멈추는 기법
     - 하지만 실제 학습시 validation 성능이 한참 오르지 않을때가 있기 때문에 주의가 필요
  3. Ensemble Methods
     - 다양한 hyper-parmeter 조절 + randomness
  4. Dropout
     - 매번 forward pass를 할 때마다 전체 parameter 중 일부를 masking
     - 모델 전체 parameter 중 일부를 이용해서도 좋은 성능을 얻을수 있도록 유도
     - batch normalizaion? 이 비슷한 효과를 낸다

## Batch Normalization

---

- 학습안정도, 학습속도에 많은 개선을 준 알고리즘
- Activation Distribution Assumption

  - 모델 자체에 대한 분석과 여러 유용한 알고리즘 대부분 activation과 parameter 분포에 대해 Gaussian을 가정
  - 실제로는 이 Gaussian 분포를 따르지 않는다. -> 학습속도가 느려지고 학습방향이 일정하지 않다.
  - 이를 위해 mini-batch 단위로 activation은 normalize 하여 원하는 분포로 만들어준다.

- Batch Normalizaition
  - 각 layer의 activation을 batch 단위로 normalize를 하여 원하는 분포로 만들어 줌
  - RNA나 lstm의 경우 모델의 특성상 사용이 어렵다.
  - 학습 과정에서는 mini-batch 전체의 정보를 이용해 batch-statics를 계산하여 normalize에 사용
