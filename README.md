[![Video Label](http://img.youtube.com/vi/tZe2udGi6is/0.jpg)](https://www.youtube.com/watch?v=tZe2udGi6is)
<br /><br />
아두이노와 LED를 사용해 5분 단위로 현재시간을 한글로 표시하는 시계 제작 프로젝트입니다.  
초 단위로 정보를 제공하는 일반적인 시계와 달리,  
**5분의 여백을 한글이라는 감성으로 제공**한다는 점이 매력으로 다가와 제작하게  되었습니다.
<br /><br />  
> 참고 : 한글시계 프로젝트 https://github.com/suapapa/HangulClock  


<br /><br />  


[주요 부품]
- Arduino Mini Pro (5V)
- LED Strip Light — SK6812 60LED
- DS3231 RTC(Real Time Clock)
- 한글 프린트 시트지/ 아크릴 판 / 소형액자
- Arduino Sketche / IDE
- 옵션) FTDI232 USB Uploader / USB C 타입 충전모듈
<br /><br />  
[주요 기능]
1) 아두이노, DS3231 RTC, SK6812 LED를 연결해 5분단위로 시간 표시
2) 보조배터리 전원을 고려하여, 저-전력 시스템을 위해 아두이노가 LED에 시간 표시 후, 5분간 슬립모드로 전환.  
   슬립모드로 전환한 아두이노를 5분마다 알람으로 깨우기 위해 RTC의 Interrupt 신호 제어
3) 5분 간격의 슬립모드 전환 작동에 있어, 최초 전원이 들어갔을 때 시간이 5분단위가 아닐 경우(e.g 1시 13분)  
   발생하는 시차동안 슬립모드 전환을 보류하는 보정코드 추가
4) 21:00 ~ 6:00 야간 시간대에는 LED 밝기를 30%이하로 낮춰 조도 조절
5) 다른 한글배치에도 코드를 쉽게 수정해 사용할 수 있도록 시간/분 배치를 5X5배열식으로 정의  

<br /><br />


<img src="https://miro.medium.com/v2/resize:fit:720/format:webp/1*Rr67gVdIPRHeREKoT9eUPw.jpeg" width="600" height="600" />
<img src="https://miro.medium.com/v2/resize:fit:720/format:webp/1*HElKKL1nrsajTlkmBc-kAQ.jpeg" width="600" height="600" />
<img src="https://miro.medium.com/v2/resize:fit:720/format:webp/1*GolG9fBZRyFIWI2--EdIHQ.jpeg" width="600" height="600" />
<img src="https://miro.medium.com/v2/resize:fit:720/format:webp/1*AyXoFwd9ebutbX86y15xkg.jpeg" width="600" height="600" />
<img src="https://miro.medium.com/v2/resize:fit:1100/format:webp/1*NPh-V1Q-p90uuFhL8oCMFQ.jpeg" width="600" height="600" />



[한글시계 제작기]
- [Arduino WordClock 한글시계(feat. chatGPT) #1.하드웨어파트](https://medium.com/@xenologue/arduino-wordclock-%ED%95%9C%EA%B8%80%EC%8B%9C%EA%B3%84-with-chatgpt-1-%ED%95%98%EB%93%9C%EC%9B%A8%EC%96%B4%ED%8C%8C%ED%8A%B8-efba34714c3a)
- [Arduino WordClock 한글시계(feat. chatGPT) #2.소프트웨어파트](https://medium.com/@xenologue/arduino-wordclock-%ED%95%9C%EA%B8%80%EC%8B%9C%EA%B3%84-with-chatgpt-2-%EC%86%8C%ED%94%84%ED%8A%B8%EC%9B%A8%EC%96%B4%ED%8C%8C%ED%8A%B8-1cebe3ce53ee)


