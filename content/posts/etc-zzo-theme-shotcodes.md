---
title: "zzo theme shortcodes"
subtitle: 
date: 2020-12-06
description: 
- "zzo theme shortcodes"
meta_image: static/images/whoami/avatar.png
tags:
- zzo theme
categories:
- etc
libraries:
- flowchartjs
---

#### hugo zzo 테마에서 사용 가능한 shortcodes
---
&nbsp;&nbsp;&nbsp;기본적으로 Markdown 문법도 제공하지만, 아래 shortcode들도 사용가능해서 블로그 쓰는데 유용할 것 같다. 이외에도 flowchartjs등 차트라이브러리도 사용가능하다.

{{< alert theme="warning" >}}
**this** is a text
{{< /alert >}}

{{< alert theme="success" >}}
**this** is a text
{{< /alert >}}

{{< alert theme="info" >}}
**this** is a text
{{< /alert >}}

{{< alert theme="danger" >}}
**this** is a text
{{< /alert >}}

{{< expand "Expand me" >}}Some Markdown Contents{{< /expand >}}

{{< img src="/images/whoami/avatar.png" title="Image4" caption="Image description" alt="image alt" width="300px" height="500px">}}

{{< notice success >}}
success
{{< /notice >}}

{{< color "#33ACFF" >}}*color*{{< /color >}}

{{< box >}}
Some contents
{{< /box >}}

{{< boxmd >}}
Some markdown contents
{{< /boxmd >}}

{{< tabs Windows MacOS Ubuntu >}}
  {{< tab >}}

  ### Windows section

  ```javascript
  console.log('Hello World!');
  ```

  {{< /tab >}}
  {{< tab >}}

  ### MacOS section

  Hello world!
  {{< /tab >}}
  {{< tab >}}

  ### Ubuntu section

  Great!
  {{< /tab >}}
{{< /tabs >}}

{{< button href="https://www.webfx.com/tools/emoji-cheat-sheet/" width="300px" height="36px" color="primary" >}}emoji-cheat-sheet{{< /button >}}

 I :heart: :fish: 

[zzo theme document](https://zzo-docs.vercel.app/)