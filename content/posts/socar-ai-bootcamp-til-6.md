---
title: "Convolutional Neural network"
subtitle: "[TIL] 쏘카 X 멋쟁이사자처럼 (AI 엔지니어 육성 부트 캠프 2기) 6주차"
date: 2022-06-14
description:
  - "AI는 어떻게 차와 사람을 구분할까요?"
image: images/socar-ai.png
meta_image: static/images/socar-ai.png
tags:
  - TIL
  - AI
  - Convolutional Filter
  - Convolutional Neural network
  - Padding, Stride - Feature map의 크기
categories:
  - 쏘카 X 멋쟁이사자처럼 AI 부트 캠프
---

## 들어가며

---

&nbsp;&nbsp;&nbsp;6주차 강의에 접어들었다. 이번 강의는 컴퓨터 비전의 핵심적인 CNN에 대한 내용이다. 컴퓨터가 이미지를 어떻게 처리하고 어떻게 학습시키는지에 대해 배우고, CNN의 구조에 대해서 자세히 알아보게 되었다. 그리고 이번에 2차 과제가 나왔다. 1차 과제는 타다의 ETA 예측모델의 MSE를 낮추는 것이었는데 과제 제출은 했지만 MSE를 기준에 맞게 줄이지 못해 좋은 점수를 받지는 못했다. 이번에는 MLP로 MNIST, FMNIST을 Finetuning 하는 것인데, Hyperparameter 세팅 조합을 하여 여러개의 모델을 테스트 해보는 것이다. 높은 점수를 받기 위해 미리 과제를 시작해봐야겠다.
{{< alert theme="success" >}}
깊이보다는 AI에 대한 전반적인 지식을 기록한 글입니다.
{{< /alert >}}

## 6주차

---

1. 컴퓨터 비전 - 컴퓨터는 어떻게 이미지를 이해할까요?
2. Convolution Filter란?
3. Convolutional Neural network의 구조
4. Padding, Stride: Feature map의 크기
5. (실습) CNN MNIST classification

## 컴퓨터 비전

---

- 인공지능의 한 분야로 이미지, 비디오 등의 시각적 입력으로부터 의미있는 정보를 얻어낼 수 있도록 하는 것

  - 이미지 분류(Image Classification)
  - 자율 주행 차량

    - 물체 감지 (Object detection)
    - 인도와 차도 구분 (Semantic segmentaition)

  - 카메라 얼굴 인식, 배경 인식
  - 차량 파손 위치 판별
  - 등등...

- 주위 배경과 구분되면서 찾기 쉬운 특징(feature 또는 keypoint)을 찾아서 비교함으로써 이미지를 인식한다.
- 복잡한 이미지의 경우에는 어떻게 구분할까?

  - ImageNet Challenge: 10,000,000장 이상의 라벨이 주어진 이미지 데이터셋인 ImageNet 데이터셋을 활용, classification, detection 등의 성능을 겨루는 대회
  - ImageNet 데이터셋은 충분히 generalized된 데이터라 인정되어 널리 쓰임
  - **CNN**을 사용한 AlexNet이 우승하게 됨

## Convolution Filter란?

---

- Convolution은 함수 f를 가만히 두고 함수 g를 t축으로 뒤집은 채로 평행하게 움직일때 각 위치에서 두 함수가 비슷한지 계산한다.
- f와 뒤집힌 g를 t만큼 이동시킨 함수가 비슷하게 생겼을수록 해당 위치를 나타내는 t에서 convolution의 값은 커진다.
- 이미지에서는 2차원 데이터에서 convolution을 적용한다.
- **Convolution filter(kernel)**: 이미지와 convolution을 통해 feature를 찾을 수 있게 하는 detector
- 이미지와 filter의 convolution을 통해 얻어진 결과값을 feature map 또는 activation map이라 한다.
- Feature map은 해당 위치에서 이미지가 filter가 나타내는 feature를 얼마나 가지고 있는지를 표현한다.
  <br>
  {{< img src="/images/socar-ai-bootcamp-til-6/convolution filter.PNG" title="convolution filter" caption="수직선 filter" position="center" width="350px" >}}

### 인공신경망에서 Convolution의 활용

- CNN은 convolution filter를 통해 feature map을 얻는 convolution layer가 있는 인공신경망이다.
- Convolution layer의 filter는 미리 정해지는 것이 아니라 학습을 통해 얻어진다.

  - c.f 이미지 filter의 경우 convolution이 아니라 비슷한 cross-correlation을 계사하는 경우도 많다.
  - 관습상 convolution으로 부른다.

### 입력 데이터에 여러 채널이 있을 때 Convolution

- 흑백 이미지: 높이(H) x 너비(W)의 2차원 데이터, 컬러 이미지(RGB): H X W X 3(Channels)
- 같은 입력값에 대해 여러 종류의 filter를 사용해서 filter에 따라 서로 다른 feature map 을 얻어낼 수 있다.
- 얻어진 feature map을 겹겹이 쌓아 filter 개수만큼의 channel을 가진 3차원 데이터로 표현한다.
  <br>
  {{< img src="/images/socar-ai-bootcamp-til-6/convolution filter2.png" title="convolution filter" caption="https://indoml.com/2018/03/07/student-notes-convolutional-neural-networks-cnn-introduction/" position="center" width="550px" >}}

## Convolutional Neural Network의 구조

---

- Convolutional layer, Activation layer(function), Pooling layer, Fully connected layer

### Convolutional layer

- Convolution filter(Cin, Cout, Hc, Wc)를 parameter로 가지는 layer
- 입력데이터(Cin x Hin x Win)와 filter로 convlution 연산하여 (Cout x Hout x Wout)을 출력함
- CNN은 Conv layer를 계층적으로 쌓아서 high-level feature를 얻음

### Pooling layer

- Feature map의 spartial size를 줄이는 layer로 conv layer 사이에 반복적으로 들어간다.
- Parameter 수를 줄여서 학습량, 계산량을 줄이고 overfitting을 억제하는 역할을 한다.
- Max pooling: 영역 내에서 가장 큰 값을 대표값으로 삼아 feature map을 요약한다.

  - 대표적으로 많이 쓰이는 방법은 2 x 2 max pooling, 2 stride 가 있다. (75% feature가 생략됨)
  - 2 x 2 max pooling 2 stride : 2 번의 sliding에 1번 operation (데이터 겹침이 없음)

- CNN을 실제로 학습할때 아래의 내용들을 이해하고 적용해야한다.

  - Parameters: 학습되는 값, Hyperparameters: 미리 정해주는 값
    <br>
    {{< img src="/images/socar-ai-bootcamp-til-6/cnn layer.PNG" title="CNN layer" caption="layer들의 비교" position="center" width="450px" >}}

### Receptive field

- Conv layer를 여러 층 거친 feature의 값은 하나의 filter의 크기보다 넓은 영역의 데이터의 정보를 담고있다.
- Receptive field: feature 값을 얻는데 사용된 입력데이터 범위의 크기

  - Layer 깊이, conv filter 크기, pooling layer, stride 등의 영향으로 달라진다.
  - 입력 이미지에 가까운 conv filter들은 receptive field가 작아 단순한 feature들을 학습하게 된다. (low-level features)
  - 입력 이미지에 멀수록 receptive field가 커지고 이전 layer에서 얻은 feature도 입력으로 받기 때문에 복잡한 feature 들을 학습하게 된다.(high-level features)

### MLP와 CNN의 비교

- Parameter 수

  - MLP의 경우 1000 x 1000 크기의 RGB 경우 하나의 feature를 뽑느데 3,000,000 개의 weight가 필요하다.
  - CNN은 filter의 parameter는 공유되므로 하나의 feature를 뽑는데 Hc x Wc x C 개의 weight만 필요하다.

- Local connectivity

  - 이미지의 경우 멀리 떨어진 위치의 정본는 도움이 안된다. 그래서 CNN이 유의미한 정보를 얻기가 유용하다.

- 이미지의 크기

  - MLP의 경우 학습될 때 사용한 이미지의 크기와 정확히 같은 크기의 입력값만 사용 가능하지만, CNN은 sliding window 형태의 filter를 학습했기 때문에, 입력값의 가로 세로 크기가 달려져도 가능하다.

## Padding, Stride: Feature map 의 크기

---

### Padding

- Convolution을 할때 input 가장자리에 추가적인 값을 넣어서 spartial size를 키우는 것
- Output feature map의 크기를 조정하기 위해 사용된다.
- 주로 padding의 값을 0으로 넣어주는 Zero-padding을 많이 사용한다.

### Stride

- Convolution에 sliding window를 어떤 간격으로 할지 나타내는 값
  <br>

---

- feature map 계산기 : https://madebyollin.github.io/convnet-calculator/
