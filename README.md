# 🎇 wordullet
빵야빵야! wordullet이 간다!

# 🎠 How to build
1. VS2010으로 .sln 파일을 여십시오.

2. 파일이라곤 있는 게 없으므로 그냥 빌드하십시오.

# 🎊 How to use
1. 개행 문자를 구분자로 사용하는 데이터 셋 `wordullet.txt`을 만드십시오. 아래는 예젱.
```
Chocolate
BAMBOO
EARL GREY
GREEN TEA
```

2. `wordullet.txt` 파일을 `wordullet.exe`와 같은 디렉토리에 옮기십시오.

3. `wordullet.exe`를 실행하십시오.

4. 원하는 곳을 더블클릭하면... 빵야빵야!

# 🍭 How it works?
윈도우 함수 `SetWindowsHookEx`를 사용하여 마우스와 키보드의 이벤트를 도청했습니다. 이것은 OS의 관점에서 매우 불쾌한 일이므로, 들키지 않도록 조심하십시오. `SetWindowHookEx`를 이용한 도청 행위는 매우 쉽게 차단당할 수 있습니다.

어찌어찌 마우스 클릭이 발생함을 알 수 있게 되었으나, 이 작자는 더블 클릭에 대해선 감지할 수 없는 무지렁이이므로 `GetDoubleClickTime`을 추가로 사용했습니다. 마우스 클릭 간의 시간을 측정해 만약 `GetDoubleClickTime`보다 작을 경우 더블 클릭이라 판단합니다.

*...귀요미로부터 >_0*

----

# 🎇 wordullet
Shoot the wordullet by double click

# 🎠 How to build
1. Open .sln file via VS2010.

2. Build it.

# 🎊 How to use
1. Make `wordullet.txt` line seperated list like below.
```
Chocolate
BAMBOO
EARL GREY
GREEN TEA
```
2. Locate the `wordullet.txt` to the same directory of `wordullet.exe`.

3. Run `wordullet.exe`.

4. Double click anywhere to write wordullet one by one.

# 🍭 How it works?
I used `SetWindowsHookEx` to hook the mouse and keyboard events. It makes me enable to detect the user's mouse clicking. However, in my case, I couldn't receive the mouse double click event for some reason (which i don't know), so I set the timer between every click event. If delay between 2 clickings is less than `GetDoubleClickTime`, that is the double click.

*...Warm Regards >_0*
