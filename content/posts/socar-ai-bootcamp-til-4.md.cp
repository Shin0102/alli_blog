---
title: "Multi-Layer Perceptron"
subtitle: "[TIL] 쏘카 X 멋쟁이사자처럼 (AI 엔지니어 육성 부트 캠프 2기) 4주차"
date: 2022-05-31
description:
  - "인공신경망, 그게 대체 뭐죠?"
image: images/socar-ai.png
meta_image: static/images/socar-ai.png
tags:
  - TIL
  - AI
  - Mult-Layer Perception
  - Deep Learning
  - Forward Pass
  - Activation Function
  - Loss Function
categories:
  - 쏘카 X 멋쟁이사자처럼 AI 부트 캠프
---

## 들어가며

---

&nbsp;&nbsp;&nbsp;4주차 강의에 접어들었다. 이제까지는 주로 Machine Learning 의 대한 내용들에 대해 다루었다. 강의의 진도가 매우 빠르다. TIL과 실습과제들을 해보면서 감을 잡고있긴한데 벅찬 것 같다. 피어그룹을 하면서 서로 진도체크 하는것이 도움이 되는 것 같다. 그리고 부트 캠프 전 기수중에 AI 엔지니어로 커리어 전환하신 분의 1시간 정도 강연을 해주셨는데, 그분도 처음에는 멘붕상태였다고 그랬다. 하지만 이해 안되는 것에 너무 집착하지 말고 최대한 강의와 과제를 따라가 결국엔 최우수 수강생이 되셨다고 하였다. 그렇게 위로아닌 위로를 받고 이제부터는 Deep Learning 에 대한 강의가 시작되는데 다시 한번 힘을 내야겠다.

{{< alert theme="success" >}}
깊이보다는 AI에 대한 전반적인 지식을 기록한 글입니다.
{{< /alert >}}

## 4주차

---

1. Multi-Layer Perception
2. Deep Learning
3. Forward Pass
4. Activation Function
5. Loss Fucntion
6. (실습) Pytorch Tutorial
7. (실습) MLP MNIST Classfication

## Multi-Layer Perception

---

- Perceptron?

  - 다차원 입력 벡터에 가중치(w)를 곱해 출력 값을 얻는 알고리즘, 벡터간의 내적 + bias

- 선형 모델의 확장, 기존 선형모델이 해결하기 어려운 문제를 해결하기 위한 모델(XOR gate problem)
- MLP 구조
  - Parameters: Weight, Bias
  - Activation Function: input과 output 관계에서 non-linearity(비선형성)을 준다. -> 좀 더 복잡한 문제를 해결
  - Loss Function
- MLP 동작 방식: Forward Pass(Parmeters 와 activation function을 이용) -> get Loss -> Backward Pass
- MLP 의 layer가 늘어나면 Parameter 수가 엄청나게 늘어나게 됨

## Forward Pass

---

- 입력이 주어졌을 때 parameter와 activation function을 통해 출력은 추론하는 과정
- Batch Training: 학습이나 추론을 할때, 하나의 데이터가 아닌 여러개의 데이터를 묶어서 진행

  - 데이터의 묶음을 **batch**라 한다.
  - 데이터를 여러개로 학습을 하게되면 좀 더 효율적으로 성능을 높일수있다.
  - 행렬과 행렬의 곱으로 확장

- Matrix Multiplication(행렬곱)?
- Mini Batch Training:

  - 모든 데이터를 가지고 하는것은 메모리나 over fitting의 문제가 있음
  - 효율적인 학습을 위해 random 하게 데이터를 sampling
  - 1 epoch => 여러개의 mini-batch를 통한 학습
  - epoch을 반복하면서 모델의 성능을 높인다.

## Activation Function

---

- Why? 각 layer의 연산은 선형 연산, 비선형 성질을 가진 activation function을 적용하여 모델이 더 다양한 표현력을 가지도록 함
- Activation Function 종류
  - Sigmoid: 미분 계수?의 계산이 간단, 미분 계수의 값이 0이 되는 영역이 너무 넓음, Vanishing Gradient Problem
  - tanh: Vanishing Gradient Problem
  - **ReLu: input value의 max operation(max(0,x)), 계산이 굉장히 빠름, 주로 많이 사용됨**
  - Leaky-Relu: ReLu의 값이 음수에서 사라지는 문제를 해결
- Softmax Function
  - 모델 output 부분에서 사용
  - 모델이 어떤 class로 추정했는지 이해하는데 도움을 줌

## Loss Function

---

- 모델의 output이 얼마나 틀렸는지를 나타내는 척도, 나중에 backward pass에서 모델의 parameter를 수정하는데 사용
- 주로 regression task는 MSE loss function을 사용
- 주로 classification task는 cross-entropy loss function을 사용
