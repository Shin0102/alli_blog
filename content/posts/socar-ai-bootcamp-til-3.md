---
title: "[TIL] 쏘카 X 멋쟁이사자처럼 (AI 엔지니어 육성 부트 캠프 2기) 3주차"
subtitle: "Unsupervised Learning"
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

## 3주차
---

1. Unsupervised Learning
2. Dimensionallity Reduction
3. Clustering
4. (실습) Data Analysis with Pandas

### Unsupervised Learning
---

- 정답 label이 없는 트레이닝셋이 주어짐
- input feature에서 의미있는 패턴 찾기
- 시각화, 전처리, 차원축소등의 데이터 분석 ,데이터 시각화
- Dimensionallity Reduction, Clustering

### Dimensionallity Reduction
---

- High-dimensional Data: 이미지, 동영상, 유전자 분석
- Curse of dimensionality: 데이터가 고차원일수록, 많은 데이터가 필요
- 불필요하게 중복되는 변수나 의미없는 변수를 줄이자
- PCA: 데이터 variance를 보존하면서 차원축소
  - 데이터의 분산을 가장 잘 설명해주는 축을 찾는다.
  - projection 이후 variance를 최대화하는 축 → Convariance matrix? 를 최대로하는
  - PVE?
  - Scree plot에서 “elobw point”를 찾거나
  - 미리 정한 크기의 분산을 설명하는 가장 작은 components를 사용
  - 한계점: classificaion에 도움이 되지않을 수 있다.(variance에 초점을 맞추기때문)
- MDS: 데이터간의 거리를 보존하면서 차원축소
- t-SNE: local neighborhood 정보를 보존하면서 차원축소
  - 데이터가 주어졌을때 neighbor일 확률은 gaussian 분포를 따른다.

### Clustering
---

- 문서, 이미지 군집화, 주식 종목 군집화, 상권 분석, 구매 패턴등
- Partitioning Clustering: 사전에 정의된 숫자의 군집중 하나에 소속
  - K-Means Clustering:
    - 각 군집은 하나의 중심을 가짐(centroid)
    - 사전에 군집의 수 K가 정해져야 함
    - SSE를 최소화 하는 partition을 찾는것 > elbow point k를 찾는것, 그 이상은 overfitting
    - 한계점: 군집의 크기, 밀도가 다르거나 구형이 아닌경우 좋지 않은 결과가 나옴
- Hierarchial Clustering: 계층적인 데이터 군집화. Dendrogram
  - Agglomerative Clustering:
    - K를 미리 정해줄 필요가 없음
    - 거리 계산방식에 따라 다양한 결합 (linkage) 방식이 있음
    - min distance, max distnace, average distance, centroid distance
    - 한계점: 계산복잡도가 크다, 군집화가 잘못되면 되돌릴 수 없다.
- Density-based Clustering: 데이터의 density를 기반으로 임의의 형태의 군집을 찾는것
  - DBSCAN: 데이터의 densitiy가 높은 영역과 그렇지 않은 영역으로 구분
