---
title: "django, 협업하기 좋은코드 만들기"
subtitle: "django clean code"
date: 2022-07-31
description:
  - ""
image: images/django-clean-code/django.svg
meta_image: static/images/django-clean-code/django.svg
tags:
  - django
  - clean code
  - style guide
  - custom exception hanlder
categories:
  - clean code
---

## 들어가며

---

&nbsp;&nbsp;&nbsp;django로 개발하게 된지도 어느덧 5년여 되어가는 것 같다.  보통 django 와 django rest framework(DRF)을 같이 사용하여 rest api 서버를 구성하는 경우가 많았다. 여러 프로젝트를 거치며 다양한 스타일의 django 프로젝트를 경험하였다.  추상화 수준이 낮은 function based view 로 구현하는 방식부터 DRF GenericViewSet을 활용하여 구현 코드를 최소화 하는 방식 등등 상황에 따라 구현방식을 적절히 선택해왔던 것 같다. 하지만 서비스가 커짐에 따라 비즈니스 로직들이 복잡해지게 되면서, 코드의 복잡성이 높아지고 유지보수가 어려워지는 경우가 발생하게 된다.  


## django style guide

---

&nbsp;&nbsp;&nbsp;이번 회사에 온보딩을 하면서 코드를 이해하는데 어려움을 느꼈는데, 먼저 핀테크에 대한 경험이 부족하다보니 관련 용어나 플로우에 대해 익숙치 않았던 점도 있지만, 코드컨벤션 없이 비즈니스 로직들이 파편화 되어 있어 코드파악이 어려웠다. model, serializer, view등 여러곳에 비즈니스 로직들이 구현되어있었고 재사용이 어려웠다. 특히나 django, drf의 높은 추상화 수준의 클래스를 상속받아 그의도와 다르게 메소드를 오버라이드 하는경우도 있어 더욱 파악이 어려웠다. 그래서 백엔드 팀의 코드컨벤션이 절실했고 이를 위해 팀 내 **‘django convention’** 문서를 작성하였다.

**django style guide**: <a href="https://github.com/HackSoftware/Django-Styleguide" target="_blank">링크</a>

&nbsp;&nbsp;&nbsp;전체적인 컨셉은 위링크 **django style guide**를 참고하였다. django project를 진행하면서 고민했던 내용들이 정리되어 있었고 그에 대한 해결책을 제시해주고 있다. 주요 내용을 정리하면 아래와 같다.

- Service(Selector) layer를 추가하여, 비즈니스 로직을 작성
    - 실제 서비스는 단순히 하나의 model 과 1:1 매핑되지 않기 때문에 이를 어떤 위치에 두어야 하는지 명확하지 않다.
    - 그렇기 때문에 별도의 layer를 통해 관리 해야 한다.
- 아래 위치에 비즈니스 로직을 놓는것은 지양
    - APIs and Views.
    - Serializers and Forms.
    - Form tags.
    - Model `save` method.
    - Custom managers or querysets.
    - Signals.
- model property에 많은 relation 로직을 넣게 되면(fat model)  이는 N+1 문제를 발생시킬 수 있다.
    - relation 로직은 serivce layer로 분리하고 직접적으로 모델과 관련된 로직을 작성
- data validation은 model objects `full_clean` 을 활용
- API & Serializer는 심플하게 유지
- Input Serializer 와 Output Serializer를 통해 data in/out 관리
- Custom Exception, logging으로 에러처리에 대한 관심사 분리


## code snippet

---

&nbsp;&nbsp;&nbsp;위 정리한 내용을 토대로 백엔드 팀에서 활용할만한 snippet을 작성해보았다. 해당 snippet을 보면 API View만 보아도 한눈에 Flow **(view → serializer → selector or service → model(manager or property))** 를 파악할 수 있다는 장점이 있다. view 에서는 사용할 selector 나 service를 import 하여 사용하면 된다. selector나 service의 특징에 따라 추상화하여 인터페이스를 정의하면 더욱 가독성 좋은 코드를 만들 수 있다. 
```python
  class {Entitiy}{Action}API(views.APIView):
      """
      Example API
      """

      class InputSerializer(serializers.Serializer):
          """
          Input Serializer
          """

          input1 = serializers.CharField()
          input2 = serializers.CharField(required=False)

          class Meta:
              ref_name = "{entitiy}_{action}"

      class OutputSerializer(serializers.Serializer):
          """
          Output Serializer
          """

          ouput1 = serializers.CharField()
          ouput2 = serializers.CharField()

          class Meta:
              ref_name = "{entitiy}_{action}"

      permission_classes = (permissions.IsAuthenticated,)

      @swagger_auto_schema(
          operation_summary="Example api action",
          operation_description="",
          request_body=InputSerializer,
          responses={
              status.HTTP_201_CREATED: openapi.Response("success", OutputSerializer),
              status.HTTP_400_BAD_REQUEST: openapi.Response("validation error"),
              status.HTTP_409_CONFLICT: openapi.Response("duplicate error"),
          },
      )
      
      def post(self, request): # or def get(self, request)
          input_serializer = self.InputSerializer(data=request.data)
          input_serializer.is_valid(raise_exception=True)
          
          # your service, selector logic Class or function
          service = ExampleService(request.user)
          data = service.something(**input_serializer.validated_data)
          
          output_serializer = self.OutputSerializer(data=data)
          output_serializer.is_valid(raise_exception=True)

          return response.Response(output_serializer.data, status=status.HTTP_201_CREATED)  

```

## Custom Exception handler

---

{{< img src="images/django-clean-code/custom-exception-handler.png" title="Custom Exception handler" caption="" position="center" width="700px" >}}
&nbsp;&nbsp;&nbsp;또 한가지 유용하게 사용하고 있는 내용으로는 exception을 handling하는 방식이다. 아래 그림을 보면 Service(Selector) layer에서 Exception 을 발생시키면 Custom handler를 통하여 Response 하게 된다. 에러나 예외케이스에 대해 일관성있게 Response 할 수 있다. 실제로 현재 Custom handler에 error_code와 error_msg를 정의하여 Response 하고 있으며, 이를 swagger에 정의하여 Frontend와의 커뮤니케이션하고 있다. 아래는 사용하고 있는 에러 response 형식이다. 
```json
  {
      "error_code": 000000,
      "error_class": "NotFoundSomething",
      "error_detail": "에러 메시지에 대한 내용",
  }
```


## 마치며

---

&nbsp;&nbsp;&nbsp;클린코딩은 여러가지 방법론과 규칙들이 존재한다. SOLID 원칙 준수, 디자인패턴 적용, 적절한 변수명, 파일명 사용하기 등등 구글링을 하면 유용한 정보들이 많이 존재한다. 하지만 개인적으로 제일 중요한건 협업하는 사람들과의 약속이라고 생각한다. 이런 약속안에서 작성된 코드는 예측이 가능해지고 코드를 파악하는데  크게 도움이 된다. 그리고 코드리뷰를 하기 위한 좋은 참고가 되기도 한다. 아직 현재 회사에서는 레거시 코드들이 남아있긴 하지만, 현재 새로 만들어지는 기능들이나 리팩토링 작업들은 위 style guide를 기반으로 작업이 되고 있고, 이를 통해 같이 일하는 팀원들이 기존 코드 파악이나 새롭게 만들어지는 기능의 가독성이 크게 좋아졌다고 말해준다. 앞으로 이 style guide를 더 고도화하여 지금보다 더 나은 코드가 되도록 해야겠다.
