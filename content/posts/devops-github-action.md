---
title: "github action으로 Kubernetes 배포 자동화"
subtitle: "github action으로 Kubernetes 배포 자동화"
date: 2020-12-30
description: 
- "github action으로 Kubernetes 배포 자동화"
image: images/github-action.png
meta_image: static/images/github-action.png
tags:
- github action
- Kubernetes
- argocd
categories:
- devops
---

## 들어가며
---
&nbsp;&nbsp;&nbsp;작년부터 회사의 거대한 레거시 시스템을 Python 기반의 API 서버로 조금씩 포팅하고 있다. 한번에 모두 포팅하기란 불가능하기에 MSA를 기반으로 여러 기능들로 나누어(메시지, 결제, 고객용 API 등등...) 포팅을 진행하였다. 하지만 처음에 배포환경이나 Framework 대한 결정없이 각각 진행하다보니, AWS EC2, Lambda, ECS등 다양한 환경에서 API 서버들이 돌아갔다. 그러다 보니 해당 모듈의 담당자가 없으면 서비스를 배포하기가 힘든 환경이 되었고, 배포를 위해서 노트북마다 환경에 맞는 세팅이 필요하게 되었다. 이러한 상황을 극복하기 위해 배포환경을 통일하는게 필요하였고, 그래서 아래와 같이 EKS 기반의 배포 자동화 파이프라인을 구축하였다.

## Kubernetes 배포 자동화
---
{{< img src="/images/eks-pipeline.png" title="배포 자동화를 위한 EKS pipeline" width="700px" position="center" >}}
{{< alert theme="info" >}}
**참고.** [MSA를 위한 Kubernetes 세팅과 CI/CD Pipeline 구성](https://medium.com/finda-tech/finda-msa%EB%A5%BC-%EC%9C%84%ED%95%9C-kubernetes-%EC%84%B8%ED%8C%85%EA%B3%BC-ci-cd-pipeline-%EA%B5%AC%EC%84%B1-%EA%B7%B8%EB%A6%AC%EA%B3%A0-monitoring-%EC%8B%9C%EC%8A%A4%ED%85%9C-%EA%B5%AC%EC%B6%95-2-ef29380ec474)
{{< /alert>}}
&nbsp;&nbsp;&nbsp;구성하는데 위 블로그의 도움을 많이 받았으며, 크게 다른점은 Jenkins 대신에 CDK로 AWS CodePipeline을 생성하여 사용하였다. 간단히 Flow를 설명하면 아래와 같다.

### flow
{{< boxmd >}}
**1. Code commit**
**2. AWS CodePipeline에서 Code commit 이벤트를 받아 Docker Build 하고 ECR에 해당 이미지를 Push**
**3. AWS CodePipeline에서 GitOps repository에 있는 helm values file을 업데이트 (이미지 태그나 secret value 등등)**
**4. AWS CodePipeline 'success' 또는 'fail'을 슬랙으로 알림**
**5. argocd 에서 Girpops repository의 변경내역이 발생하면, 변경된 내용을 기반으로 자동(or 수동)으로 Kubernetes내의 Resource를 업데이트 한다.**
{{< /boxmd >}}
{{< img src="/images/argocd-ui.png" title="실제 운영중인 gui" width="700px" position="center" >}}
&nbsp;&nbsp;&nbsp;실제로 위와 같이 배포 파이프라인을 구축하고 난 뒤에는 배포에 대한 부담이 많이 줄어들었다. 배포를 위한 다른 세팅이나 Kubernetes Cluster 접근할 필요없이 argocd gui를 통해 배포 & 롤백을 매우 편하게 할 수 있게 되었기 때문이다. 하지만 프로젝트를 새로 생성 할때마다, **AWS CodePipeline**을 추가를 해줘야하는 수고스러움?이 발생한다. 물론 CDK를 이용하여 이것도 자동화를 하였지만, 매번 AWS CodePipeline을 생성해야 된다는 것이 그렇게 반가운 상황은 아니었다. 그렇게 고민하던중 **github action**을 알게 되었고, 파이프라인 구축을 좀 더 간소화 할 수 있을것 같은 기대감을 가지고 세팅을 시작해보았다.(github action을 써보고 싶기도 하였다..)

## github action
---
&nbsp;&nbsp;&nbsp;**github action**이란 github에서 제공하는 CI/CD 툴이다. 사용법은 간단하다. gitub project에 Actions 탭에 들어가면 새로운 workflow를 만들 수 있다. yml 파일로 만들어지며, 생성된 파일은 default로 .github/workflows/{name}.yml 위치하게 된다. 자세한 설명은 잘 정리해주신 블로거가 많으니 참고하면 될 것 같다. 일단 해야될 work을 정해보면 아래와 같다.
{{< boxmd >}}
1. docker build
2. AWS ECR push
3. GitOps repository의 Kubernetes deployment.yaml image tag를 update
{{< /boxmd >}}


GitOps repostroy의 Kubernetes deploymnet.yaml의 image tag에 빌드된 이미지의 태그가 업데이트 되면, GitOps repository와 연동해놓은 argocd 에서 자동(or 수동)으로 Cluster에 해당하는 Pod를 업데이트 하여 sync를 맞춘다. 일단 위 작업들을 정의한 github action은 아래와 같다. 
- project name : {{< color "#33ACFF" >}}*테스트*{{< /color >}}라 가정.
- gitops repo 아래 경로에 k8s yaml(service, deployment)이 정의되어있다고 가정.
  - {{< color "#33ACFF" >}}*gitops repo*{{< /color >}}/dev/{{< color "#33ACFF" >}}*테스트*{{< /color >}}/resources.yaml
  - {{< color "#33ACFF" >}}*gitops repo*{{< /color >}}/prod/{{< color "#33ACFF" >}}*테스트*{{< /color >}}/resources.yaml
- (github action을 수행하기전에 아래 값들을 github secrets에 세팅필요)
{{< expand "Github Secrets" >}}
- AWS_ACCESS_KEY_ID_VAL
- AWS_SECRET_ACCESS_KEY_VAL
- ECR_REPOSITORY -> AWS ECR Repository 이름
- GIT_ACCESS_TOKEN -> gitops git repsitory를 checkout 하기위한 Token
{{< /expand >}}
### 예제
```yml

on:
  push:
    branches: 
      - master
      - feature

name: Docker Image build & k8s resources.yaml update

jobs:
  deploy:
    name: Deploy
    runs-on: ubuntu-latest

    steps:
    - name: Checkout
      uses: actions/checkout@v2
    
    - name: Set Dev env variables
      if: endsWith(github.ref, '/feature')
      run: |
        echo "Feature branch"
        echo "ENVIRONMENT=dev" >> $GITHUB_ENV
        echo "DESTINATION=dev/테스트" >> $GITHUB_ENV
        echo "BRANCH=feature" >> $GITHUB_ENV
    
    - name: Set Prod env variables
      if: endsWith(github.ref, '/master')
      run: |
        echo "Master branch"
        echo "ENVIRONMENT=prod" >> $GITHUB_ENV
        echo "DESTINATION=prod/테스트" >> $GITHUB_ENV
        echo "BRANCH=master" >> $GITHUB_ENV
    
    - name: Set commit msg 
      env:
        GITHUB_SHA: ${{ github.sha }}
      run: |
        echo "COMMIT_MSG=Update from https://github.com/your/테스트/$GITHUB_SHA" >> $GITHUB_ENV
        echo $COMMIT_MSG
    #
    # Push app image to ECR
    #
    - name: Configure AWS credentials
      uses: aws-actions/configure-aws-credentials@v1
      with:
        aws-access-key-id: ${{ secrets.AWS_ACCESS_KEY_ID_VAL }}
        aws-secret-access-key: ${{ secrets.AWS_SECRET_ACCESS_KEY_VAL }}
        aws-region: ap-northeast-2

    - name: Login to Amazon ECR
      id: login-ecr
      uses: aws-actions/amazon-ecr-login@v1

    - name: Build, tag, and push image to Amazon ECR
      id: build-image
      env:
        ECR_REGISTRY: ${{ steps.login-ecr.outputs.registry }}
        ECR_REPOSITORY: ${{ secrets.ECR_REPOSITORY }}
        IMAGE_TAG: ${{ github.sha }}
      run: |
        docker build -t $ECR_REGISTRY/$ECR_REPOSITORY:$ENVIRONMENT-$IMAGE_TAG {Dockerfile 경로}
        docker push $ECR_REGISTRY/$ECR_REPOSITORY:$ENVIRONMENT-$IMAGE_TAG
        echo "::set-output name=image::$ECR_REGISTRY/$ECR_REPOSITORY:$ENVIRONMENT-$IMAGE_TAG"
        
    #
    # Update Image tag to delployment.yaml
    #
    - name: Git config
      run: |
        git config --global user.email "actions@github.com"
        git config --global user.name "GitHub Actions"
        
    - name: Check out k8s repo
      uses: actions/checkout@master
      with:
        repository: your/gitops-repository
        token: ${{ secrets.GIT_ACCESS_TOKEN }}
        
    - name: chmod repo destination
      run: |
        chmod +x $DESTINATION
        ls
        
    - name: git push image tag to k8 repo 
      env:
        GITHUB_REPO: ${{ github.repository.name }}
        GITHUB_SHA: ${{ github.sha }}
      run: |
        sed -i -e 's@${{ steps.login-ecr.outputs.registry }}/${{ secrets.ECR_REPOSITORY }}:.*@${{ steps.build-image.outputs.image }}@g' $DESTINATION/resources.yaml
        git add .
        git commit -m "$COMMIT_MSG"
        git push -f --set-upstream origin main
```
&nbsp;&nbsp;&nbsp; 구조를 보면 **트리거 / build 환경 / 실제 수행할 step**들이 정의되어 있다. feature, master branch 에 push될때 dev와 production 서버를 업데이트 하기위한 github action이다. 라인별 설명은 아래와 같다.
{{< boxmd >}}
**line 19~33**  : GitOps repository에 해당 프로젝트의 kubernetes 리소스들이 있는 path를 지정해준다.(dev, prod)
**line 39**     : GitOps에 commit할 msg 지정 -> 현재 commit의 url link를 commit msg로 하기위해 repository url을 세팅해준다.
**line 55~64**  : docker build하고 지정한 ECR에 push
**line 74~78**  : GitOps repository checkout
**line 80~83**  : 실행권한 추가
**line 85~93**  : k8s deployment가 정의된 yaml 파일의 tag를 업데이트
{{< /boxmd >}}

## AWS CodePipeline과 비교
---
&nbsp;&nbsp;&nbsp;일단 기존의 사용하던 codepipe line과 비교해보면 **사용법이 굉장히 쉬웠다**. AWS Codepipeline도 iac tool 을 이용해서 쉽게 세팅할 수 있지만, 초기 세팅이 쉽지는 않았다. 그리고 github action이 좋았던 것이 **브랜치별로 pipeline을 구성할 필요가 없다**는 것이 굉장히 편리했다. step에서 조건문을 이용하면 branch 별로 action을 제어할 수 있다. 마지막으로 빌드 시간에서 생각보다 많은 차이가 났다. 기존 CodePipeline을 사용했을때는 5~6분정도 소요 되었는데 github action은 **절반도 안되는 시간이(2분 정도) 소요**되었다. 
{{< alert theme="info" >}}
**참고.** [GitHub Actions vs. AWS CodePipeline](https://cloudonaut.io/github-actions-vs-aws-codepipeline/)
{{< /alert>}}

## 마치며
---
&nbsp;&nbsp;&nbsp;AWS CodePipeline에서 github action으로 cd pipeline을 변경하면서 구성이 굉장히 가벼워진 느낌이 들었다. 프로젝트 내에서 branch별로 pipeline을 만들 필요도 없고, 간단히 프로젝트 repository에 workflow가 정의된 yml파일만 추가해주면 되기 때문이다. 결과적으로는 github을 repository로 사용하고 있고, AWS Resource에 의존성이 크지 않으면 github action을 사용하지 않을 이유는 없을 것 같다.