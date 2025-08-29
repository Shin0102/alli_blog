---
title: "Unsupervised Learning - Dimensionallity Reduction, Clustering"
subtitle: "[TIL] 쏘카 X 멋쟁이사자처럼 (AI 엔지니어 육성 부트 캠프 2기) 3주차"
date: 2022-05-25
description:
  - "Unsupervised Learning, Dimensionallity Reduction, Clustering"
image: images/socar-ai.png
meta_image: static/images/socar-ai.png
tags:
  - TIL
  - AI
  - Unsupervised Learning
  - Dimensionallity Reduction
  - Clustering
categories:
  - 쏘카 X 멋쟁이사자처럼 AI 부트 캠프
---

## 들어가며

---

&nbsp;&nbsp;&nbsp;3주차 강의에 접어들었다. 이번 강의는 Machine Learning중 output이 주어지지 않는 **Unsupervised Learning**에 대한 내용이다. 생소한 수식들과 처음듣는 용어들 때문에 이해하기가 힘들지만, 너무 집착하지말고 최대한 숲을 보려고 노력중이다. 그래도 실습에서 실제 데이터를 가지고 진행하다보니 좀 더 이해하기가 수월하였다. 그리고 이번 부트캠프에서는 Peer Group을 정해주는데, 아무래도 온라인으로 진행하다보니 서로의 진도를 체크하고, Study에 있어 서로에게 도움을 주는 것이 목적인것 같았다. 아무래도 취준생이나 학생분들이 많이 참여한 Group들이 많이 보였다. 다행히도 직장인분들이 많은 Group이 있어 그곳에 참여하여 만남을 진행하였고, 다양한 분야의 사람들을 만나보고 같이 Study 하는 기회를 가지게되어 좋았던 것 같다.
{{< alert theme="success" >}}
깊이보다는 AI에 대한 전반적인 지식을 기록한 글입니다.
{{< /alert >}}

## 3주차

---

1. Unsupervised Learning
2. Dimensionallity Reduction
3. Clustering
4. (실습) Data Analysis with Pandas

## Unsupervised Learning

---

- 정답 label이 없는 트레이닝셋이 주어짐
- output을 예측을 하는것이 목표가 아니라, input feature에서 의미있는 패턴 찾기가 목적이다.
- 시각화, 전처리, 차원축소등의 데이터 분석 ,이상 탐지등의 목적으로 사용
- Dimensionallity Reduction, Clustering

## Dimensionallity Reduction

---

- High-dimensional Data: 추천시스템(users \* movies), 이미지, 동영상, 유전자 분석
- **Curse of dimensionality**: 데이터가 고차원일수록,같은 성능의 모델 학습을 위해 많은 데이터가 필요
- 불필요하게 중복되는 변수나 의미없는 변수를 줄이자

  1. PCA(Principal Compo): 데이터 variance를 보존하면서 차원축소

     - 데이터의 분산을 가장 잘 설명해주는 축을 찾는다.
     - projection 이후 variance를 최대화하는 축 → Convariance matrix? 를 최대로하는
     - PVE?
     - ㄴ Scree plot에서 “elobw point”를 찾거나, 미리 정한 크기의 분산을 설명하는 가장 작은 components를 사용
     - 한계점: classificaion에 도움이 되지않을 수 있다.(variance에 초점을 맞추기때문)

  2. MDS: 데이터간의 거리를 보존하면서 차원축소
  3. t-SNE: local neighborhood 정보를 보존하면서 차원축소, 차원에서 멀리 떨어져있는 데이터는 신경을 별로 쓰지않고 가까이 있는 데이터들이 차원축소 후에도 가까워져 있기를 기대한다.

     - 데이터가 주어졌을때 neighbor일 확률은 gaussian 분포를 따른다.

  4. Auto-encoder, Word2Vec: 딥러닝 기반의 차원 축소

## Clustering

---

- 문서, 이미지 군집화, 주식 종목 군집화, 상권 분석, 구매 패턴등
- Partitioning Clustering: 사전에 정의된 숫자의 군집중 하나에 소속

  - K-Means Clustering:
    - 각 군집은 하나의 중심을 가짐(centroid)
    - 사전에 군집의 수 K가 정해져야 함
    - SSE를 최소화 하는 partition을 찾는것 > elbow point k를 찾는것, 그 이상은 overfitting
    - 한계점: 군집의 크기, 밀도가 다르거나 구형이 아닌경우 좋지 않은 결과가 나옴
      <br>

- Hierarchial Clustering: 계층적인 데이터 군집화. Dendrogram
  - Agglomerative Clustering:
    - K를 미리 정해줄 필요가 없음
    - 거리 계산방식에 따라 다양한 결합 (linkage) 방식이 있음
    - min distance, max distnace, average distance, centroid distance
    - 한계점: 계산복잡도가 크다, 군집화가 잘못되면 되돌릴 수 없다.
      <br>
- Density-based Clustering: 데이터의 density를 기반으로 임의의 형태의 군집을 찾는것
  - DBSCAN: 데이터의 densitiy가 높은 영역과 그렇지 않은 영역으로 구분
