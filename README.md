# Spaceship War
Họ và tên: Đỗ Văn Đức<br>
MSV: 24021410<br>

I. Introduction
- Game lấy ý tưởng từ game Pong, 1 game khá nổi tiếng những năm mà máy tính trò chơi arcade còn phổ biến
- Đây là game đầu tiên em làm nên không thể tránh khỏi những bug tiềm ẩn khi xây dựng ạ :((
- Điểm mong muốn đạt được: 9.5

II. Gameplay
- Người chơi điều khiển 2 ship: redShip và blueShip, đẩy bullet qua cho nhau.
- Mỗi round đều random angle di chuyển của bullet ( sang phải / trái và lên trên / xuống dưới ) ( giải thích ở dưới )
- Bullet khi va chạm với thuyền sẽ tăng dần vận tốc (theo phương va chạm đó, dx, hoặc dy, hoặc cả 2 (khi va chạm theo cả 2 phương, khi đó rơi vào edge case)), và tuân theo định luật phản xạ ( edge case: khi bullet va chạm ở rìa ngoài ship, sẽ random góc bật ra theo phương ngược lại ) (giải thích chi tiết ở phần III)
  ( Còn khi va chạm với 2 tường thì chỉ bật lại 1 góc phản xạ bằng góc tới lúc trước, vì chỉ đổi chiều dx và k tăng vận tốc theo phương đó )
- Có 2 chế độ
  + Chế độ PVP: 2 người chơi điều khiển 2 ship đẩy bullet qua cho nhau, người chơi nào hết máu trước sẽ thua
  + Chế độ PVE: 1 người điều khiển và đấu với máy<br>*Tỉ lệ 30% sẽ có event round xuất hiện ngẫu nhiên tùy ván, event round là những round có 2 bullets thay vì 1 <br>

III. Algorithm Introduction
- Ý tưởng thuật toán:
  + Đạn di chuyển theo 2 trục Ox, Oy. Ta sẽ xét trên từng trục, nếu có va chạm sẽ đổi chiều vận tốc trên trục đó <br>
  ( Trước khi đổi chiều, ta đẩy hẳn bullet cả khỏi khối collier va chạm, sau đó mới đổi chiều vận tốc để tránh bug va chạm liên tục ) <br>
  + Trong 1 frame, hiểu đơn giản cơ chế viên đạn di chuyển thì đây là phép cộng 2 vecto theo chiều dx và dy sẽ ra vecto tổng hợp là chiều di chuyển của viên đạn ( có thể hiểu là đi chéo ) <br>
  + Nếu va chạm theo cả 2 trục thì thuật random góc phản xạ sẽ chạy, gen ra góc bật ngẫu nhiên trong [20, 60] degree ( tránh góc 0 hoặc 90 để tránh đạn bay vertically / horizontally ), góc bật ở đây là góc giữa tia bật ra ( tổng hợp của dx và dy sau khi đảo dấu vì va chạm cả 2 phương nên nó đảo dấu trước rồi hàm này mới chạy ) và tia dx ( kiểu phép cộng vecto trong toán học đã nói ở trên ) ( vẫn giữ nguyên dấu âm nếu dx hoặc dy âm lúc trước, đạn sẽ bật ngược lại với góc random mới chứ k theo định luật phản xạ nữa )<br>
  + Hàm random góc này cũng được gọi mỗi round ( tương đương random ra dx và dy mới )
  ( vì cos(alpha) > 0 và sin(alpha) > 0 với alpha là góc nhọn --> Khi nhân vào speed ( luôn dương ) sẽ luôn dương --> đạn sẽ bay lên và sang phải, sau đó ta tung đồng xu để xác định dấu ngẫu nhiên theo cả 2 chiều ( trái + lên / trái + xuống / phải + lên / phải + xuống ) <br>

  * Trong chế độ PVE:
  - Người chơi điều khiển redShip, máy điều khiển blueShip thông qua thuật toán:
  + Nếu k có event (tức là 1 bullet) thì chỉ đơn giản là nếu đạn ở bên trái so với blueShip thì blueShip sẽ bay sang trái ( check khoảng cách trên trục Ox ), còn bên phải thì bay sang phải (có 1 vùng delay distance để làm cho thuyền máy bị trễ 1 chút khi đuổi theo bullet (nhưng k đáng kể))
  + Còn khi có 2 bullet, ta duyệt 2 bullet để check khi có bullet bay về phía nó, và máy sẽ tìm khoảng cách Euclid ngắn nhất từ tâm blueShip đến tâm của 2 bullet, và sẽ target vào bullet đó, sau khi target thì tiếp theo ý tưởng giống khi xử lý 1 bullet.

IV. Graphics introduction
- Có menu, màn hình dừng game, tutorial
- Các nút đều có hiệu ứng âm thanh khi hover (di vào) và khi click, và đổi màu
- Khi pause có thể chọn replay hoặc trở về menu
- Logo tắt tiếng ở menu và khi pause game cũng có thể tắt các sound trong game (vd tiếng collision, gamepoint, hay background_music )
- Có màn hình GameOver
- Animation sparkle nhấp nháy đầu menu

V. Source tham khảo
- 2 shipTexture và bulletTexture là tự vẽ trên https://www.pixilart.com/ ( để có thể biết rõ các collider để xử lý va chạm ) ( chỉ xử lý va chạm ở 3 hình chữ nhật đã tô màu ở dưới đáy Ship )
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

