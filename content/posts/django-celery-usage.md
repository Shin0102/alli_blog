---
title: "django 에서 celery 활용기"
subtitle: ""
date: 2022-04-19
description:
  - "실제 프로젝트에서 사용한 경험 공유"
  - "trouble shooting"
image: images/django-celery-usage/python-celery.png
meta_image: static/images/django-celery-usage/python-celery.png
tags:
  - django
  - celery
  - 비동기
  - async
  - 분산락
  - distributed lock
  - redis
  - visibility timeout
categories:
  - django
---

## 들어가며

---

&nbsp;&nbsp;&nbsp;django로 서버를 운영하다보면 비동기 처리가 필요한 경우가 많다. 예를 들어 외부 API 호출, 데이터베이스 조회, 복잡한 계산 등등 많은 경우에 비동기 처리가 필요하다. 이때 celery를 활용하여 비동기 처리를 할 수 있다. celery에 관한 기본적인 설정이나 사용법들은 다른 블로그에 잘 나와있어서 이 포스트에서는 실제 프로젝트에서 사용한 경험과 유용했던 설정, 그리고 trouble shooting을 정리해보려 한다. (Broker 서버는 redis를 사용)

## celery 구성

---

&nbsp;&nbsp;&nbsp;아래는 celery를 실제 서비스에 운영하면서 설정한 내용들이다.
##### 1. 큐 분리
&nbsp;&nbsp;&nbsp;celery는 broker서버에 있는 **task들을 worker가 pull 해서 처리**한다. 이때 오랜시간이 걸리는 task를 모든 worker가 큐에서 prefetch 하여 가져와 작업(대기)중인 상태라면 다른 task가 실행되지 못하고 쌓이게 된다. 이럴 경우 task의 특성에맞게 큐를 분리해주면 task가 쌓이는 상황을 어느정도 방지할 수 있다. 아래는 간단한 예시이다.

```python
app.conf.task_default_queue = "short-task"  # 기본 큐 설정
app.conf.update(
    task_queues=[
        Queue('short-task', routing_key="short.#"),  # 빠른 처리가 필요한 작업용 큐
        Queue('long-task', routing_key="long.#"),    # 시간이 오래 걸리는 작업용 큐
    ]
)

# 태스크 예시
@app.task(queue='short-task')
def send_notification():
    pass

@app.task(queue='long-task')
def process_large_file():
    pass
```

##### 2. prefetch-multiplier 및 acks_late 설정
&nbsp;&nbsp;&nbsp;먼저 각각의 설정에 대해 알아보면 ***prefetch-multiplier***는 큐에서 가져오는 task의 수를 결정하는 설정이다. 기본값은 4이며, 이 말은 worker가 큐에서 task를 가져올때 4개의 task를 가져옴을 의미한다. ***acks_late***는 True로 설정하게 되면 task가 성공적으로 실행되었을때만 브로커에서 해당 task를 삭제한다. 기본값은 False 이다. 이 옵션들은 일반적으로 아래와 같이 활용하는게 좋다.

{{< boxmd >}}
- short-running task는 prefetch-multiplier를 높게 설정하여 빠른 처리를 위해 더 많은 task를 가져오게 세팅
- long-running task는 prefetch-multiplier를 1로 설정하여 한번에 하나의 task만 가져오게 세팅하여 **worker를 점유하는 시간을 줄이고** acks_late를 True로 설정하여 task가 정상적으로 실행되지 않았을 경우 브로커에서 해당 task를 삭제하지 않도록 세팅 (**이때 중복처리에 대한 처리가 되어있어야 함, 멱등성 보장**)
{{< /boxmd >}}

##### 3. Ofair
&nbsp;&nbsp;&nbsp;***Optimized Fair Scheduling***을 의미하며, 작업을 처리할 때 task를 실행가능한 worker를 바로 할당을 한다. 따라서 작업들의 실행 시간이 제각각인 경우에도 최적화된 실행을 할 수 있다. (4.0부터 기본으로 동작함)

## djang-celery-beat

---

&nbsp;&nbsp;&nbsp;djang-celery-beat는 celery의 task를 주기적으로 실행할 수 있도록 관리하는 플러그인이다. 별도의 코드없이 django admin에서 쉽게 task 스케줄을 관리할 수 있기 때문에, celery를 사용하는 경우 유용하게 사용할 수 있다. crontabl, interval, periodic 여러방식으로 설정할 수 있다. [https://github.com/celery/django-celery-beat](https://github.com/celery/django-celery-beat)


## single task 처리

---

&nbsp;&nbsp;&nbsp;celery를 사용하다보면 task가 중복실행되는 경우를 방지하고싶은 상황이 생긴다. 예를들어 특정 task의 실행시간을 예측하기 힘든상황에 주기적으로 task가 실행되어야 한다면 중복실행을 방지하는 것이 중요하다. 이때 application level에서 중복실행을 방지하기는 어렵다. 특히나 worker서버가 다수인 경우에는 더욱 그렇다. 그래서 ***redis 분산 lock***을 활용하였다. 예제는 아래와 같다. 

```python
# task의 worker 실행 개수를 1개로 제한하기 위한 redis lock decorator
def single_task(timeout: int = TASK_LOCK_EXPIRE) -> Callable:
    def task_exc(func: Callable) -> Callable:
        @functools.wraps(func)
        def wrapper(*args: tuple, **kwargs: dict) -> None:
            lock_id = TASK_LOCK_ID_PRE_FIX + func.__name__
            acquire_lock = lambda: cache.add(lock_id, "true", timeout)  # noqa
            release_lock = lambda: cache.delete(lock_id)  # noqa
            if acquire_lock():
                logging.info(f"Acquired lock for {func.__name__}")
                try:
                    func(*args, **kwargs)
                except Exception as e:
                    logging.info(
                        f"Exception raise {e} lock for {func.__name__}"
                    )
                finally:
                    release_lock()
                    logging.info(f"Released lock for {func.__name__}")

        return wrapper

    return task_exc


# 사용 예시
@app.task()
@single_task()
def precoess_something():
    pass
```
위와 같이 사용하면 task의 중복실행을 방지할 수 있다. 주의할점은 lock expire 시간을 충분히 길게 잡아주어야 한다. 추가적으로 worker 서버 배포시에 ***task lock을 expire 시키는 script를 작성***하여 실행시켜줘야 한다.
- 참고: [https://stackoverflow.com/questions/4095940/running-unique-tasks-with-celery](https://stackoverflow.com/questions/4095940/running-unique-tasks-with-celery)


## trouble shooting

---

#### redis broker visibility timeout
&nbsp;&nbsp;&nbsp;데이터를 마이그레이션 할 일이 생겨, 1회성으로 celery task를 script를 실행하였는데, 어느순간부터 데이터 중복이 모니터링 되었다. 이상해서 flower 를 확인해보았는데 task가 중복으로 실행되고 있었다. 

&nbsp;&nbsp;&nbsp;문제는 ***redis의 visibility timeout 문제였다(+acks_late=True)***. redis를 broker로 사용하는 경우 메시지를 영속적으로 저장하지 않고, visibility timeout 에 의존한다. 해당값은 1시간(3600)이 기본값으로 설정되어있다. 해당 script가 1시간 이상 소요되면서 task가 중복실행 되었던 것이었다. redis의 경우 설정이 간단한 대신 메시지가 영속적으로 저장되지 않기때문에, 메시지 손실이 발생할 가능성이 있다. 그래서 celery에서는 RabbitMQ를 사용하는 것을 권장하고 있다.