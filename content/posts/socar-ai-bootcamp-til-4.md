---
title: "[TIL] 쏘카 X 멋쟁이사자처럼 (AI 엔지니어 육성 부트 캠프 2기) 4주차"
subtitle: "인공신경망, 그게 대체 뭐죠?"
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

## 4주차
---

1. Mult-Layer Perception
2. Deep Learning
3. Forward Pass
4. Activation Function
5. Loss Fucntion
6. (실습) Pytorch Tutorial
7. (실습) MLP MNIST Classfication

### Mult-Layer Perception
---

- Perceptron?: 입력 벡터에 가중치(w)를 곱해 출력 값을 얻는 알고리즘, 벡터간의 내적 + bias
- Forward Pass -> get Loss -> Backward Pass
- 선형 모델의 확장, 기존 선형모델이 해결하기 어려운 문제를 해결하기 위한 모델(XOR gate problem)
- MLP 구조
  - Parameters: Weight, Bais
  - Activation Function
  - Loss Function

### Forward Pass
---

- 입력이 주어졌을 때 paramete와 activation function을 통해 출력은 추론하는 과정
- Batch Training: 하나의 데이터가 아닌 여러개의 데이터를 묶어서 진행
  - 데이터의 묶음을 batch라 한다.
  - 행렬과 행렬의 곱으로 확장
- Matrix Multiplicatio: 2차원 행렬들의 곱
- Mini Batch Training: 
  - 모든 데이터를 가지고 하는것은 메모리나 over fitting의 문제가 있음
  - 효율적인 학습을 위해 random 하게 데이터를 sampling
  - 1 epoch => 여러개의 mini-batch를 통한 학습


### Activation Function
---

- Why? 각 layer의 연산은 선형 연산, 비선형 성질을 가진 activation function을 적용하여 모델이 더 다양한 표현력을 가지도록 함
- Activation Function 종류
  - Sigmoid: 미분 계수?의 계산이 간단, 미분 계수의 값이 0이 되는 영역이 너무 넓음 
  - tanh
  - ReLU: input value의 max operation, 계산이 굉장히 빠름
  - Leaky-Relu: ReLu의 값이 음스에서 사라지는 문제를 해결
- Softmax Function
  - 모델 output 부분에서 사용
  - 모델이 어떤 class로 추정했는지 이해하는데 도움을 줌

### Loss Function
---

- 모델의 output이 얼마나 틀렸는지를 나타내는 척도, 나중에 backward pass에서 모델의 parameter를 수정하는데 사용
- regression task > MSE
- classification task > cross-entropy 
