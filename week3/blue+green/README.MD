# 프로그램의 실행순서

* 1.Main.cpp의 WinMain함수가 호출됨

* 2.윈도우 객체를 생성하고(window_main) 윈도우를 출력한뒤 이벤트 처리 루프를 돈다.

* 3.이벤트의 처리는 mainwindow.cpp의 LRESULT CALLBACK WndProc에서 이루어지며,
이벤트가 발생하면 wParam을 통해 어떤 메뉴 혹은 툴버튼이 눌러졌는지 알 수 있다.

* 3-1. 만약 LOAD버튼이 눌러졌다면, OFN구조체에 파일정보를 저장한다.
정상적으로 파일이 불러와졌다면, child_window객체를 생성하고 OFN을 전달해주며, 별도의 윈도우를 생성해준다.

* 3-2. 다른 버튼들이 눌러졌다면, 전역변수를 통해 어떤 버튼들이 눌러져있는지 저장해주고, 
그 전역변수를 보고, child_window의 프로시져가 이벤트를 처리해준다.

ex)main_window의 움직이기 버튼 눌려짐 + child_window에서 마우스 이동이 발생: object를 움직임  

링크해야하는 라이브러리
gdi32
user32
kernel32
comctl32
comdlg32
opengl32


추가되어야 하는것들-

윈도우 깜빡임, 툴바 위로 window가 올라가는 문제 해결 .

parsing + rendering 구현 ( childwindow.cpp )
