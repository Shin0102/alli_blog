---
title: "CNN, Autoencoder, Upsampling, Semantic Segmentation"
subtitle: "[TIL] 쏘카 X 멋쟁이사자처럼 (AI 엔지니어 육성 부트 캠프 2기) 7주차"
date: 2022-06-21
description:
  - "AI는 어떻게 사진에서 차의 위를 찾아낼까요?"
image: images/socar-ai.png
meta_image: static/images/socar-ai.png
tags:
  - TIL
  - AI
  - CNN
  - AlexNet
  - VGGNet
  - ResNet
  - Autoencoder
  - Upsampling
  - Semantic Segmentation
categories:
  - 쏘카 X 멋쟁이사자처럼 AI 부트 캠프
---

## 들어가며

---

&nbsp;&nbsp;&nbsp;7주차 강의에 접어들었다. 이번 강의에서는 대표적인 CNN 모델들과 모델에 사용되는 대표적인 작동방식(Residual Block, Bottleneck Block, Autoencoder...)에 대해서 배웠다. 그리고 이미지 Object detection과 Semantic Segmentation에 대해서 배웠다. 많은 내용들이 한 강의에 담겨있다보니, 설명들이 친절하지 않아 개인적인 공부는 필수인 것 같다. 그리고 이제 다음 8주차 강의를 마지막으로 해커톤 팀구성이 시작된다. 4~5명으로 팀을 이뤄 AI 관련 주제를 가지고 프로젝트를 진행하고, 마지막 발표회를 마지막으로 부트 캠프는 종료된다. 해커톤 팀에 도움이 될 수 있도록 강의를 잘 마무리 해야겠다.
{{< alert theme="success" >}}
깊이보다는 AI에 대한 전반적인 지식을 기록한 글입니다.
{{< /alert >}}

## 7주차

---

1. 대표적인 CNN 모델들 - AlexNet, VGGNet, ResNet
2. Object Detection - CNN을 어떻게 활용할까요?
3. Autoencoder 와 Upsampling
4. Semantic Segmentation 은 어떻게 할까요?
5. (실습) Autoencoder 다뤄보기
6. (실습) CNN pretrained model 활용하기

## 대표적인 CNN 모델들 - AlexNet, VGGNet, ResNet

---

### AlexNet

- CNN의 기반을 닦은 모델
- GPU를 사용한 CNN 학습, Activation 함수로 ReLU 사용, Data augmentation 활용, CNN에 dropout 도입
- Data augmentation?

  - 데이터의 label을 유지한채로 transform 시켜서 데이터셋의 크기를 키우는 방법
  - Overfitting 방지, generalization 성능 향상에 기여
  - 상하좌우 반전, 노이즈 삽입, random crop, blur

### VGGNet

- 많은 Layer의 개수 -> conv, fc layer 총 19개: 144 million parameters
- Conv filter 크기는 3x3 으로 고정

  - 장점: 3x3 conv layer를 여러개 쌓으면 더 적은 parameter 개수로 layer를 쌓을 수 있다.
  - ReLu(non-linearlity)가 많이 들어갈 수 있어서 표현력이 좋아짐

- 3x3 size conv filter, stride=1, padding=1 인 conv layer를 기본으로 사용함

### ResNet

- Conv layer가 많아진다고 성능이 꼭 좋아지는 것은 아니다.

  - Gradient vanishing / exploding
  - Layer 들간의 연결이 너무 길어져서 발생하는 문제

- Residual block을 이용하여 conv layer가 많은 네트워크에 대해서 학습을 성공

  - Skip connection을 이용

  #### Residual Block

  - Residual(잔차): 입력값과 출력값의 차이
  - Skip connection을 이용하여 이전 layer의 feature를 직접 받아올 수 있게 되었다.
    <br>
    {{< img src="/images/socar-ai-bootcamp-til-7/residual block.webp" title="Residual Block" position="center" width="450px" >}}

  #### Bottleneck Block

  - 1x1 convolution을 활용하여 학습 parameter수와 연산량을 줄이는 구조, ResNet-50 이상에서 주로 사용한다.
  - 1x1 convolution: 연산량이 작아 feature map의 channel을 늘리거나 줄일 때 사용한다.
  - Bottleneck block 단계

    1. 1x1 conv로 feature map channel 축소
    2. 축소된 channel에 3x3 conv로 spartial한 feature map을 얻음
    3. 1x1 conv로 feature map의 channel 원상복구
       <br>
       {{< img src="/images/socar-ai-bootcamp-til-7/bottleneck block.PNG" title="Bottleneck Block" caption="" position="center" width="450px" >}}

- 경량화 모델: MobileNet, SqueezeNet, ShuffleNet, ...

## Object Detection - CNN을 어떻게 활용할까요?

---

- 물체가 어디에 있는지와 종류를 알아내는 task

  - 위치: Bounding Box로 표현한다. (x, y, w, h)
  - 종류: Classification

### 성능 측정

- IoU(Intersection over union): 실제영역과 예측한 영역의 교집합/합집합
- Precision & Recall

  - True Positive(TP): 실제로는 있는데 있다고 올바르게 예측
  - False Positive(FP): 실제로는 없는데 있다고 틀리게 예측
  - False Negative(FN): 실제로는 있는데 없다고 틀리게 예측
  - True Negative(TN): 실제로 없는데 없다고 올바르게 예측
  - **Precision**(정밀도): TP/TP+FP -> 찾은 것들 중 실제로 있는것이 얼마나 되는가?
  - **Recall**(재현율): TP/TP+FN -> 실제로 있는것 중 찾은것이 얼마나 되는가?

- Precision-Recall 곡선(PR 곡선)

  - Confidence threshold에 따른 precision과 recall
  - Confidence : 알고리즘이 예측에 확신을 가지는 정도 -> threshold 이하의 confidence를 가지는 예측은 무시
  - Confidence threshold가 낮아지면 detection이 많아지고 pricision이 낮아지고 recall이 높아짐
  - Confidence threshold가 높아지면 detection이 적어지고 pricision이 높아지고 recall이 낮아짐

- Average Precision(AP)

  - Pricision과 recall을 동시에 고려한 성능지표
  - PR 곡선의 아래 영역의 넓이

- mean Average Precision(mAP)

  - 각 class당 AP의 평균

### Pretrained CNN 모델

- 이미지를 학습 시키려면 굉장히 오래 걸린다. -> pretrained model을 이용하여 task에 맞게 finetuning 한다.(작은 lr)

  - R-CNN -> Fast R-CNN -> Faster R-CNN
  - SPPNet
  - YOLO
  - SSD

## Autoencoder 와 Upsampling

---

- Semantic Segmentation

  - 모든 pixel에 대한 classification (pixelwise classification)
  - Pixcel 개수만큼 CNN 돌리는 것은 비효율적
  - 한번에 여러 pixel에 대한 결과를 얻고 싶다.

- Encoder-Decoder 구조

  - Encoder: 입력 데이터에서 의미있는 Latent feature(Encoder에 의해 압축된 입력 데이터의 feature)를 추출하는 네트워크
  - Decoder: Feature로부터 원하는 결과 값을 생성하는 네트워크

- Autoencoder

  - 입력과 출력이 같은 구조를 가진 인공신경망
  - Label 없이도 입력데이터의 feature를 추출할 수 있음
  - Feature 차원 축소 -> 중요한 정보만 살리기

- Decoder의 upsampling

  - 학습 parameter가 없는 경우: Bilinear interpolation, Nearest neighbor
  - 학습 parameter가 있는 경우 : Transposed convolution (Deconvolution)

## Semantic Segmentation은 어떻게 할까요?

---

- 성능측정: mean IoU, bounding box 대신 pixel로 IoU 계산 -> class 별 IoU의 평균
- 많이 쓰는 데이터셋: PASCAL VOC 2012, MS COCO, Cityspaces
- FCN(Fully Convolutional Network)

  - fc layer 없이 conv layer로만 구성됨 -> 입력 이미지 크기에 제약받지 않음
  - VGG16을 backbone newtwork로 사용, VGG16의 fc layer를 conv layer로 변환
  - VGG16의 fc layer feature만으로 upsampling 하면 corse한 정보만 담김
  - 중간 conv feature map의 정보를 사용하여 디테일들을 추가

- DeconvNet

  - Deconvolution layer도 convolution layer만큼 많이, 대칭적으로 쌓아보자

- U-Net

  - skip connection
  - Encoder의 conv feature map을 대응되는 크기의 decoder layer에 직접 전달

- DeepLab V3

  - Atrous convolution(Dilated convolution)
  - Conv filter 사이를 확장하여 convolution 하는 방법
  - 넓은 Receptive field를 가짐 -> spartial 정보에 대한 손실을 줄일수 있다.
  - Atrous Spartial Pyramid Pooling(ASPP)
    - Atrous rate가 다른 여러 filter를 사용하여 multi-scale 정보를 획득
