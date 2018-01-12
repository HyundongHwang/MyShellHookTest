# MyShellHookTest

<!-- TOC -->

- [MyShellHookTest](#myshellhooktest)
    - [목적](#%EB%AA%A9%EC%A0%81)
    - [원리](#%EC%9B%90%EB%A6%AC)
    - [스크린샷](#%EC%8A%A4%ED%81%AC%EB%A6%B0%EC%83%B7)
    - [주의사항](#%EC%A3%BC%EC%9D%98%EC%82%AC%ED%95%AD)
    - [참고자료](#%EC%B0%B8%EA%B3%A0%EC%9E%90%EB%A3%8C)

<!-- /TOC -->

## 목적
- adb 를 실행한 결과를 win32 app에서 얻어 오는 POC 코드
- 범용성을 위해 닷넷보다는 win32 코드를 사용했다.

## 원리
- adb의 출력이 `Standard Output` 에 기록되기 때문에 윈도우쉘에서 프로세스를 실행후 `Standard Output` 을 ReadFile 하면 된다.

## 스크린샷
![](https://s13.postimg.org/pxj7ejfbr/screenshot_116.png)

## 주의사항
- adb.exe 가 설치된 경로를 자신의 환경에 맞게 코드를 약간 수정해야 한다.

## 참고자료
//Creating a Child Process with Redirected Input and Output
//https://msdn.microsoft.com/ko-kr/library/windows/desktop/ms682499(v=vs.85).aspx