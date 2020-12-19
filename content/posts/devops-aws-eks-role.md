---
title: "AWS EKS Role에 대한 고찰"
subtitle: 
date: 2020-12-19
description:
meta_image: static/images/whoami/avatar.png
tags:
- aws eks
- cdk
- role
- iam
categories:
- devops
---

---
&nbsp;&nbsp;&nbsp;MSA(microservice architectur)를 위해 CDK(typescript)를 통해 EKS를 구성한 적이 있는데, 이때는 동작에만 집중하다 보니, 자동으로 생성되는 리소스나 Role들에 대해 크게 신경 쓰지 않았다. 그러다 이번에 번잡하게 관리되는 AWS Resource들에 대해 정리할 기회가 생겨, 새로 EKS 환경을 구성할 기회가 생겼다. 그중에 이번에는 Role에 대해 정리해보려 한다. 

--- 
&nbsp;&nbsp;&nbsp;CDK를 통해 eks를 생성하게 되면 role에 대해 특별히 명시하지 않아도, 자동으로 IAM Role들이 몇 개 만들어진다. Cluster에 대한 Role, 그리고 같이 생성되는 Lambda에 대한 Role도 여러 개 있다.(Lambda 관련된 것들은 다음에 정리해봐야겠다) 먼저 Cluster 관련 Role을 정리해 보면 아래와 같다.
{{< alert theme="info" >}}
Kubernetes v1.18, CDK 1.78.0/ 세팅에 따라 이름이나, Role 개수가 다를 수 있음
{{< /alert>}}

{{< boxmd >}}
**1. {cluster-name}-ClusterCreationRole**
**2. {cluster-name}-admin-role**
**3. {cluster-name}-eks-role**
**4. {cluster-name}-ClusterNodegroupNodesNodeGroupRole**
{{< /boxmd >}}

#### 1. {cluster-name}-ClusterCreationRole
---
&nbsp;&nbsp;&nbsp;먼저 **{cluster-name}-ClusterCreationRole**을 살펴보면, ClusterCreationRoleDefaultPolicy라는 인라인 Policy(이것도 자동으로 생성됨)가 연결되어있다. 정책 내용에는 EKS Cluster를 생성에 사용할 Subnet과 VPC에 대한 정보에 대한 접근 허용, Cluster 생성/삭제/버전업데이트 등 클러스터 생성에 관한 권한들이 나열되어 있다. 내용이나 이름으로 보아 cdk에서 cluster 생성 및 관리를 하려는 Role 인 것 같다.

#### 2. {cluster-name}-admin-role
---
&nbsp;&nbsp;&nbsp;**{cluster-name}-admin-role**은 확인해보면 아무런 Policy도 연결되어 있지 않다. 그래서 좀 찾아보니 먼저 EKS인증 방식을 알아야 한다.
{{< img src="/images/eks-rbac.png" title="AWS 인증방식" caption=https://docs.aws.amazon.com/ko_kr/eks/latest/userguide/managing-auth.html width="600px" position="center" >}}
&nbsp;&nbsp;&nbsp;kubectl 을 통해 Cluster에 접근하게 되면 해당 유저가 적절한 I AM User인지 확인한 후, 인증되면 k8s(Kubernetes)의 RBAC을 이용해 접근제어를 하게 된다. 
{cluster-name}-admin-role은 Cluster를 초기 생성하는 User에게  system:groups라는 그룹을 부여하게 된다. 이 그룹은 k8s Default ClusterRole인 cluster-admin(super user)이라는 ClusterRole과 바인딩되어 있다. 그래서 Cluster 초기 생성자는 Cluster관한 거의 모든 권한을 가지고 있다.
(**참고.** [k8s default cluster role and role bidings](https://kubernetes.io/docs/reference/access-authn-authz/rbac/#default-roles-and-role-bindings))

#### 3. {cluster-name}-eks-role
---
&nbsp;&nbsp;&nbsp;**{cluster-name}-eks-role**은 AmazonEKSClusterPolicy라는 기존 Policy가 연결되어 있는데, 설명을 보면 아래와 같다.
{{< boxmd >}}
This policy provides Kubernetes the permissions it requires to manage resources on your behalf. Kubernetes requires Ec2:CreateTags permissions to place identifying information on EC2 resources including but not limited to Instances, Security Groups, and Elastic Network Interfaces. 
{{< /boxmd >}}
&nbsp;&nbsp;&nbsp;k8s 에서 AWS 리소스들을 관리하기 위해 필요한 권한들이라는 설명이다. Policy를 확인해보면 EC2, Load Balancer, AutoscalingGroup 등 서비스 운영에 주요한 리소스들에 대한 권한들이 포함되어 있다. AWS Console에 들어가보면 생성한 Cluster에 해당 Role이 연결되어 있는것을 확인할 수 있다.

#### 4. {cluster-name}-ClusterNodegroupNodesNodeGroupRole
---
&nbsp;&nbsp;&nbsp;마지막으로 ;**{{cluster-name}-ClusterNodegroupNodesNodeGroupRole**을 확인해보면, 총 3가지의 AWS Policy가 연결되어있다.
- AmazonEKSWorkerNodePolicy : EKS Worker node를 EKS Cluster에 연결하기 위한 Policy
- AmazonEC2ContainerRegistryReadOnly : ECR(컨테이너 저장소) Readonly Policy
- AmazonEKS_CNI_Policy : EKS에서 Conatiner 들의 Network Interface들을  관리하기 위해 EKS는 AWS CNI라는 plugin을 사용하는데, 해당 플러그인에게 제공하는 Policy 인 것 같다.

&nbsp;&nbsp;&nbsp;확인해 보니 Container Image를 읽어와 Worker Node안에 Pod들을 배포할때 필요한 정책들이 들어가 있다. 실제로 Worker Node들은 EC2 Instance에 해당하고, Pod들은 이 Worker Node들 안에 적절히? 배포된다.

---

&nbsp;&nbsp;&nbsp;이제 Role들에 대해 알아봤으니, 실제 구성한 환경에서 확인해보자. 아래 명령어로 eks 생성시 만들어진 aws-auth configmap을 확인할 수 있다.
```bash
kubectl edit -n kube-system configmap/aws-auth
```
{{< img src="/images/capture-configmap.png" caption="aws auth configmap" width="800px" position="center" >}}
&nbsp;&nbsp;&nbsp;처음 eks를 생성하면 aws configmap이 위와 같이 정의되어 있다. (실제로 String 형태로 되어있었지만 가독성을 위해 위 형식으로 수정하였다.). 위에서 설명한 Workder Node관련 Role과 Cluster 초기 생성자와 관련된 Role이 보인다. mapUsers에는 원래 아무것도 없었지만, EKS 생성후에 AWS Console EKS에서 Nodegroup에 대한 정보가 제대로 나오지않고 **'Your current user or role does not have access to Kubernetes objects on this EKS cluster'** 에러가 나와서 로그인한 I Am user에 master 권한을 추가해주니 정상적으로 Nodegroup 정보가 출력되었다. 다른유저들이 EKS Cluster에 접근하게 추가하고싶으면 mapUsers부분에 추가해주면 된다.

&nbsp;&nbsp;&nbsp;이글은 아래 내용을 많이 참고하였고, EKS 환경을 구성하게 된다면 꼭 한번 보기를 추천한다. Custom Role과 Group을 만드는 내용도 있어 다음에 필요하면 세팅해볼 예정이다.
[쿠알못이 Amazon EKS로 안정적인 서비스 운영하기](https://www.youtube.com/watch?v=O3znWPUdt18)