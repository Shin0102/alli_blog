---
title: "AWS DMS를 활용한 NoSQL 적용기"
subtitle: "AWS DMS를 활용한 NoSQL 적용기"
date: 2024-05-27
description:
  - "AWS DMS CDC"
  - "NoSQL"
image: images/nosql-apply-using-aws-dms/aws-dms.png
meta_image: static/images/nosql-apply-using-aws-dms/aws-dms.png
tags:
  - aws
  - dms
  - cdc
  - nosql
categories:
  - 성능개선
---

---

&nbsp;&nbsp;&nbsp;최근 회사에서 대량의 데이터를 엑셀로 다운로드하는 기능의 성능개선 요청이슈가 있었다. 워낙 많은양의 데이터를 한번에 불러오는 작업이다보니 DB에 병목현상이 발생하였다. DB로는 MySQL을 사용하고 있었는데, 기존의 DB 정규화가 과도하게 되어있는데다가 다운로드 해야할 데이터가 워낙 많아서 쿼리최적화나 스케일링으로 드라마틱한 효과를 볼 수 없을거 같았다. 또한 역정규화나 이벤트기반 데이터 싱크방법을 생각해봤지만, 다른 프로젝트도 진행해야 하는 상황에서 많은 시간을 할당할 수 없었고 기존 코드 수정은 최대한 피하고 싶었다. 
&nbsp;&nbsp;&nbsp;문제 해결을 위한 방법을 찾던중, **AWS의 DMS**를 알게 되었다. 이를 활용하여 CDC 데이터를 NoSQL로 마이그레이션 해보기로 결정하였고, 빠르게 PoC를 진행하고 Production 까지 배포할 수 있었다. 그 결과 기존에 비해 속도가 **수십배 가량** 향상되는 효과를 얻을 수 있었다. 

- 자세한 도입내용에 대한 링크: https://tech.paywatch.co.kr/tech/26
<br>
---