# Spaceship War
Họ và tên: Đỗ Văn Đức<br>
MSV: 24021410

I. Introduction
- Game lấy ý tưởng từ game Pong, 1 game khá nổi tiếng những năm mà máy tính trò chơi arcade còn phổ biến
- Đây là game đầu tiên em làm, không thể tránh khỏi những bug tiềm ẩn khi xây dựng :((
- Điểm mong muốn đạt được: 9.5

II. Gameplay
- Người chơi điều khiển 2 ship: redShip và blueShip, đẩy bullet qua cho nhau.
- Mỗi round đều random angle di chuyển của bullet ( sang phải và lên trên / xuống dưới ) ( giải thích ở dưới )
- Bullet khi va chạm với 2 thuyền sẽ tăng dần vận tốc (theo phương va chạm đó, dx, hoặc dy, hoặc cả hai), và tuân theo định luật phản xạ ( edge case: khi bullet va chạm ở rìa ngoài ship, sẽ random góc bật ra )
  ( Còn khi va chạm với 2 tường thì chỉ bật lại 1 góc phản xạ bằng góc tới lúc trước, vì chỉ đổi chiều dx và k tăng vận tốc theo phương đó )
- Có 2 chế độ
  + Chế độ PVP: 2 người chơi điều khiển 2 ship đẩy bullet qua cho nhau, người chơi nào hết máu trước sẽ thua
  + Chế độ PVE: 1 người điều khiển và đấu với máy<br>*Tỉ lệ 30% sẽ có event round xuất hiện ngẫu nhiên tùy ván, event round là những round có 2 bullets thay vì 1 <br>
    ** Game có thể giật lag tùy thuộc vào cấu hình máy do em k xử lý fps ạ :(. Nếu cô chơi thấy đạn hoặc thuyền di chuyển quá chậm cô có thể tăng thông số để cho mượt hơn ạ **

III. Algorithm Introduction
- Ý tưởng thuật toán:
  + Đạn di chuyển theo 2 trục Ox, Oy. Ta sẽ xét trên từng trục, nếu có va chạm sẽ đổi chiều vận tốc trên trục đó <br>
  ( Trước khi đổi chiều, ta đẩy hẳn bullet cả khỏi khối collier va chạm, sau đó mới đổi chiều vận tốc để tránh bug va chạm liên tục )
  + Khi bắt đầu mỗi round thì đều random góc bật của bullet
  + Nếu va chạm theo cả 2 trục thì thuật random góc phản xạ sẽ chạy, gen ra góc bật ngẫu nhiên từ [-65, -25] hoặc [25, 65] degree, góc bật ở đây là góc bật ra so với phương của Ox (dấu của dx), góc âm thể hiện góc bật ngược chiều lại ( tức dy < 0 ) <br>
  ( vì cos(-alpha) = cos(alpha) còn sin(-alpha) = - sin(alpha) --> đạn sẽ bay sang bên phải còn lên trên hoặc xuống dưới tùy góc âm hay dương random ra )

  * Trong chế độ PVE:
  - Người chơi điều khiển redShip, máy điều khiển blueShip thông qua thuật toán:
  + Nếu k có event (tức là 1 bullet) thì chỉ đơn giản là nếu đạn ở bên trái so với blueShip thì blueShip sẽ bay sang trái ( check khoảng cách trên trục Ox ), còn bên phải thì bay sang phải (có 1 vùng deadzone, em đặt tên biến hơi khó hiểu ạ :(, thật ra k có deadzone vẫn thắng được máy vì ở đây tốc độ đạn tăng dần, nên tâm blueShip sẽ k thể theo kịp tâm viên đạn di chuyển được)
  + Còn khi có 2 bullet, ta duyệt 2 bullet để check khi có bullet bay về phía nó, và máy sẽ tìm khoảng cách Euclid ngắn nhất từ tâm blueShip đến tâm của 2 bullet, và sẽ target vào bullet đó, sau khi target thì tiếp theo ý tưởng giống khi xử lý 1 bullet.

IV. Graphics introduction
- Có menu, màn hình dừng game, tutorial
- Các nút đều có hiệu ứng âm thanh khi hover (di vào) và khi click, và đổi màu
- Khi pause có thể chọn replay hoặc trở về menu
- Logo tắt tiếng ở menu và khi pause game cũng có thể tắt các sound trong game (vd tiếng collision, gamepoint, hay background_music )
- Có màn hình GameOver

V. Source tham khảo
- 2 shipTexture và bulletTexture là tự vẽ trên https://www.pixilart.com/ ( để có thể biết rõ các collider để xử lý va chạm )
- Các texture khác (máu, trái tim, ingameBackground, menuBackground) lấy từ nhiều nguồn trên mạng, Chat GPT gen ảnh,...
- Animation sparkle lấy từ https://itch.io/
- Âm thanh lấy từ https://freesound.org/, font chữ lấy trong file docs hướng dẫn sdl2 lập trình nâng cao cơ bản
- Nguồn học chính:
  + Docs lập trình nâng cao cơ bản được cung cấp
  + Glusoft: https://glusoft.com/sdl2-tutorials/
  + Lazyfoo ( mainly Motion, Timing, MouseEvent, Per-Pixel Collision, Scrolling,.. )
  + Sử dụng AI: Tham khảo idea, gen ảnh, rút ngắn 1 số đoạn code rườm rà mà ý nghĩa tương đương ( ở mức độ hiểu và tự triển khai phù hợp với cấu trúc game của mình )

VI. Checklist<br>
Dùng các lệnh vẽ hình ✅<br>
Texture ✅<br>
Background ✅<br>
Event bàn phím ✅<br>
Event chuột ✅<br>
Animation (hoạt hình) ✅<br>
Xử lý va chạm ✅<br>
Sound ✅<br>
Sound on off ✅<br>
Background music ✅<br>
Font ✅<br>
Menu ✅<br>
Pause/Resume ✅<br>
Status bar / lives (mạng/máu...) ✅

