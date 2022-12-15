---
title: "어떤 문제들을 AI와 딥러닝으로 해결할 수 있을까요?"
subtitle: "[TIL] 쏘카 X 멋쟁이사자처럼 (AI 엔지니어 육성 부트 캠프 2기) 8주차"
date: 2022-06-28
description:
  - "어떤 문제들을 AI와 딥러닝으로 해결할 수 있을까요?"
image: images/socar-ai.png
meta_image: static/images/socar-ai.png
tags:
  - TIL
  - AI
  - Transfer Learning
  - Ensemble Learning
  - Self-supervised Learning
  - Grad-CAM
  - GAN
  - RNN
  - Attention Model
categories:
  - 쏘카 X 멋쟁이사자처럼 AI 부트 캠프
---

## 들어가며

---

&nbsp;&nbsp;&nbsp;마지막 8주차 강의이다. 이번 강의에서는 다양한 딥러닝 모델과 그것들을 어디에 활용하는지에 대해 배웠다. 각 모델들의 자세한 설명보다는 간단간단한 컨셉만 짚고 넘어가는 강의였다. 이렇게 총 8주간의 강의는 모두 끝이났고 이제는 마지막 3차과제와 해커톤 프로젝트 만이 남았다. 3차 과제는 쏘카에서 제공해준 차량이미지를 가지고 파손 부위를 찾아내는 Semantic-Segmentation 모델을 fine-tuning하는 것이다. 재밌으면서 어려운 과제가 될 것 같다. 그리고 이제 해커톤 프로젝트가 5주동안 시작된다. 나는 '차량 운전자 졸음 및 부주의 탐지' 프로젝트 팀에 참가하게 되었다. 마지막 유종의 미가 될 수 있도록 조금만 더 힘내보자.
{{< alert theme="success" >}}
깊이보다는 AI에 대한 전반적인 지식을 기록한 글입니다.
{{< /alert >}}

## 8주차

---

1. 학습된 딥러닝 모델을 어떻게 활용할까요?
2. Self-supervised Learning: 스스로 Label을 정하는 학습
3. Grad-CAM: CNN 모델의 예측을 어떻게 설명할까요?
4. GAN: 진짜 같은 이미지는 어떻게 만들어낼까요?
5. RNN: 순차적인 데이터는 어떻게 처리할까요?
6. Attention Model: 어디를 주목할까요?
7. (실습) CNN model의 활용

## Transfer Learning / Ensemble Learning

---

- 딥러닝 모델은 학습에 많은 비용과 시간이 필요하다.

  - 이미 학습된 모델을 잘 활용해보자

### Fine-tuning(Transfer Learning)

- 이미 학습된 모델 parameter를 활용하여 현재 풀려는 문제에 맞게 약간의 추가학습을 하는 것
- 기존모델이 현재 풀려는 문제와 연관이 있을때 사용한다.
- 기존 parameter들을 크게 변화시키지 않도록 작은 learning rate를 작게 사용한다.(기존의 1/10 이상)
- 모델 전체를 업데이트하는 경우: 세 데이터셋이 충분히 많고 데이터의 분포가 기존과 다를 경우
- 아 부분은 freeze, 뒷부분만 업데이트하는 경우: 새 데이터셋이 작거나 데이터의 분포가 기존과 많이 유사할 경우

### Ensemble

- 인공신경망의 랜덤성을 활용
- 여러 모델들의 예측을 합치는 방법

  - Hard voting: 최종 예측 결과를 합산
  - Soft voting: 예측된 확률 값을 합산
  - Weighted voting: 각 모델별로 가중치를 정한다. (모델의 정확도를 weight로 준다.)

## Self-supervised Learning

---

- Labling이 된 대량의 데이터는 모으기가 힘들다.
- Label이 없는 데이터로 supervised learning을 하는 것처럼 인공신경망을 학습시키는 방법이 없을까?
- Self-supervised learning의 과정

  - 사용자가 직접 만든 pretext task를 정의
  - Label이 없는 데이터를 변형하여 pretext task를 학습할 수 있는 supervision을 생성
  - 생성한 supervsion으로 pretext task에 대한 학습

- 대표적인 예시로 Autoencoder가 있다.
  - Image noisy, colorization, inpainting, Rotation
- Self-supervised Learning을 이용한 Transfer Learning
  - Self-supervised Learning을 통해 학습시킨 모델을 pretrained network로 사용하여 downstream task의 기반으로 활용
  - Autoencoder로 예를들면 latent feature를 뽑는 encoder를 활용

## Grad-CAM

---

- 블랙박스 모델 : 입력을 주면 출력이 나오지만 그 내부 작업을 명확하지 않는 모델

  - 인공신경망이 학습한 feature들이 무엇인지를 파악하고싶다.
  - 판단의 근거를 알수없으면 안전성의 문제가 생길수 있다.
  - 사람이 생각하지 못한 판단의 근거를 알아낼 수 있다.

- Explainalble AI

### Grad-CAM

- CNN의 판단 근거를 visualize 하는 방법
- Classification class node 기준으로 feature map 상의 어떤 위치가 해당 node에 영향을 많이 줬는지 표시
- 픽셀의 위치에서 클래스 c에 대해 영향력을 많이 가지는 feature의 값이 클수록, Grad-CAM의 값이 커지는 구조
- 모델의 성능을 어떻게 높일수 있을지, 데이터를 튜닝할것인지를 판단하는데 도움이 된다.

## GAN

---

- 식별 모델(Discriminative model)

  - 데이터 X가 주어졌을때 label Y가 나타날 조건부확률 학습
  - X의 label을 잘 구분하는 decision boundary를 학습하는 것이 목표

- 생성 모델(Generative model)

  - 데이터 X의 분포 P(X), 또는 P(X,Y)를 학습
  - 데이터의 분포를 학습하여 새로운 데이터를 생성한는 것이 목표

### GAN

- 진짜같은 데이터를 생성하는 generative model
- 두개의 네트워크로 구성: Generator <-> Discriminator
- Disciriminator의 정확도가 50%가 되는 훌륭한 Generator를 학습하는 것
- Adversarial Loss: Discriminator와 Generator를 동시에 학습하는 loss
- GAN의 활용

  - Super-resolution: 해상도 개선
  - Style transfer
  - 그림으로 사진 만들기
  - 딥페이크

## RNN

---

- 순차적인 데이터 (Sequential Data)

  - 순서가 의미가 있으며, 순서가 달라질 경우 의미가 손상되는 데이터
  - ex) 자연어 번역, 음성 인식, 주가 예측

### RNN

- Sequential Data 처리에 특화된 인공신경망
- Hidden layer에 자기 자신으로 연결되는 recurrent connection이 있음
- Hidden State

  - recurrent connection의 출력, 순차적인 메모리의 역할을 한다.
  - 입력으로 부터 얻어진 feture를 다음 time step으로 전해주는 역할을 한다.

- 입력 출력의 형태에 따라 여러가지 구성이 가능하다.
  - one to one, one to many, many to one(문장 감정 분석), many to many(문장 번역, 비디오 분류)
- 대표적인 RNN 모델: LSTM(Long-Term Dependency), GRU(LSTM의 내부구조를 간소화)
- RNN 활용
  - 자연어 번역, 주가 예측, 로봇 컨트롤

## Attention Model

---

- Sequence to Sequence 모델

  - Enocder는 RNN을 통해 context vector를 출력
  - Decoder는 context vector를 이용해 RNN으로 output sequence를 생성
  - input이 길어질수록 context vector가 담을 수 있는 한계가 생기게 된다.

### Attention Model

- sequence를 순서대로 거치며 얻어낸 context vector 대신 입력 sequence에서 중요한 부분들에 집중에서 decoder로 전달
- Transformer

  - 가장 대표적으로 많이 쓰이는 attention model
  - Input Embedding, Positional Encoding
  - 개인적으로 공부해보자

- Attention Model 활용
  - 자연어 번역
  - 문장 생성
  - ...
