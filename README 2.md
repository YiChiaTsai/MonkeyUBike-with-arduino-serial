# MonkeyUBike

1032學期資科系大三專題總報告
「結合 Arduino 科技的微笑互動 U-Bike」
指導老師：蔡子傑老師 姓名: 蔡⼀嘉 YI-CHIA, TSAI 系級: 資訊科學系三年級 Email:101703017@nccu.edu.tw 日期: 6/18/2015 (Thur)

（一）1032本學期進度列表： （為方便老師檢視，紅色為沒進度、藍色為重點突破)

01/15～02/15/2015寒假： 請教MCLab各學長Arduino相關知識、幾篇paper碩論的閱讀與國科會計畫書撰寫；並於5/29/2015收到審核通過通知。

03/19/2015 (Thur)： (1) Arduino tone()－藉由蜂鳴器實作出Mario song. 除了手機APP的“情境音樂”之外，這是一個簡易的方式來達到腳踏車情境提示音。簡單的提示聲容易實作，但是歌曲則必須很熟悉Do, Re等音階，不過網路上很多範例資源可以參考。 (2) analogWrite()－練習類比的輸入，可以達到0～5V之間的需求，而非數位的HIGH& LOW而已。 (3) 中國版LED onWheel date time picture－如下圖
 
04/02/2015 (Thur)： 手機iOSAPP學習，內容包括 (1) 向余老師的研究生申請MCLab的iOS APP教育單位開發者權限，而得以將iOS APP編譯至手機內。 (2) HelloWorld, GetMyLocation, SpeechDemo三個專案練習。

04/09/2015 (Thur)： (1) 新增SpeechDemo的調整功能及中文發音。(如右圖) (2) 已可順利移植至手機。 (3) 欲修正GetMyLocation執行、新增CameraApp以及LoginSampleFB (4) 預計實作出手機播放器，搭配Arduino的蜂鳴器進行提示音及導覽。如果有相關framework可以引用，期類似Siri的辨識語音功能。

04/16/2015 (Thur)： 期中考前一週報告考試太多，此週沒進度...。

04/23/2015 (Thur)： 期中考週暫停一次。

04/30/2015 (Thur)： 補足04/16, 04/23三週的報告量，主要兩大進度為 BLE4.0& 九軸IMU。 (1) Serial(序列通訊)的概念。 (2) Bluetooth Low Energy(BLE4.0)。以手機連上 HM10控制三個if判斷式：LED on& off, Timer countdown , Mario music。(如右圖) (3) Inertial Measurement Unit (IMU)，完成測試IMU 的3個三軸。(氣壓計、陀螺儀、加速器和指南針) (4) 預計將九軸讀取到的數據傳至手機端APP進行計算。以及達到雙向傳輸、不受RX/TX困擾。 此點已於下週解決完。

05/7/2015 (Thur)： 改善上週的問題，進一步地運用IMU數據。 (1) 使九軸IMU傳資料給手機APP的同時，能順便接收APP的指令，且過程不會互相衝突、中斷。 (2) 腳踏車配有裝置的概念想像圖－ 一台腳踏車備有1個Arduino、3個BLE(HM10 or ModiaTech)、1個九軸(MPU9250 or popolu)；其中2個BLE為一主一從(BikeC, BikeF)為了達到腳踏車的雙向溝通以及RSSI不斷的強弱訊號傳送，也會嘗試Change mode的方式看是否能減少為1個；另一個則永遠設定為從(BikePhoneF)負責讓手機能與Arduino溝通。 

05/14/2015 (Thur)： (1) 紹瑾加入。 (2) 研究閱讀HM10的43頁規格書，嘗試了許多AT command的指令。AT+NAME?, AT+MODE?, AT+IMME?, AT+DISC?, etc. （此時接法為Arduino TX－HM10 TX， Arduino RX－HM10 RX）。 (3) 藉由AT+CON[Mac Address]連接起兩個Arduino，然而此時需換插拔成Arduino TX－HM10 RX， Arduino RX－HM10 TX，才能繼續跑程式碼。

05/21/2015 (Thur)： 收到Monkeylectric pro，硬體安裝、Mac軟體及APP載入Playlist測試。 Mode: 多種模式，包含BLE loading mode為燈號三以及13個playlist的1～13燈號，以白色的LED燈號位子為判斷依據。（拍攝的影片在資料夾內）

05/28/2015 (Thur)： (1) 遭遇上個月底一樣的困難，AT command會搶到Arduino上的TX/RX(這次真的互相衝突了)，但是我們的需求是在不換插拔時，達到程式碼也能同時跑AT與if判斷式。 (2) 後來使用的是#include <SoftwareSerial.h> SoftwareSerial BTSerial(gRxPin, gTxPin); //來讓軟體模擬出兩個新的TX/RX解決衝突。

06/04/2015 (Thur)： (紹瑾負責在程式碼端就下AT+CON[Mac Address]與AT+DISC? 來進行BLE連線動作) 我與Monkeylectric的人員聯絡，收到相關的Mac Tool SDK。研究了一個禮拜 (1) 可以藉由SDK interface在console下perl指令，把圖片類檔build成.img，然後download到Monkeylectric。（達到如同APP載入圖片的效果） (2) 然而SDK的README寫道REAL TIME CONTROL，那些command似乎就像是APP可以轉換這些圖片之間的命令效果。 (3) 預計六月底再寄信詢問GoldWater此SDK的所有使用方法、能否提供APP的操控Monkeylectric片段的Interface source code以及Arduino的應用讀的懂此份Perl語言的SDK嗎？

06/11/2015 (Thur)： 因為我們上上週已經可以藉由程式碼端自動下AT來連到藍芽從裝置，而非手動打AT。但是並不知道腳踏車每一次在路上遇到的從裝置Mac Address， 所以 (1) 藉由觀察AT+DISC？這個AT command掃描到附近藍芽Mac& Name的回傳方式。然後利用indexof(), substring(), trim()等C函式庫來獲取附近BLE Mac Address。 (2) 設定timer來達到每五秒鐘搜尋一次！（更好的做法？ 之後會想出較符合實際情境的搜尋間隔！）

06/15/2015 (Thur)： 系上於06/15在3301教室，舉辦大三專題提案報告繳交與專題面授課程。Demo簡報在此週資料夾。

06/18/2015 (Thur)： 1032學期資科系大三專題總報告－ 「結合 Arduino 科技的微笑互動 U-Bike」。

06/25/2015 (Thur) 期末考週暫停一次。 


（二）暑假的進度規劃：

Arduino： (1) 讓Arduino可以建多個TX/RX通道，接上多個藍牙模組並達到各自的功能。 (2) 讓Arduino 可以跟monkey 連線並下指令。 (3) 在大仁樓前設計3～5個BLE情境，觀察連線的狀態及流暢度，修正Arduino 連線資料傳輸，並改善接收資料。（可以連到別的Arduino，並分析回傳的資料） (4) 讓Arduino 可以連接三個藍牙模組，兩個雙向連線其他Arduino（景點，其他腳踏車等），另一個連線monkey。 (5) 做到可以利用Arduino連到其他裝置以後，可以把回傳的資料變成指令傳給Monkey。

MonkeyLectric： (1) 寄信詢問Goldwater此SDK的所有使用方法、能否提供APP的操控Monkeylectric片段的Interface source code以及Arduino的應用讀的懂此份Perl語言的SDK嗎？ (2) 達到Arduino或iOSAPP的Real time control。 (3) 設計CS、NCCU logo。

MPU9250的使用： (1) Transform Raw Value into Real Value。 (2) 探討Real Value的計算與應用應該在Arduino或iOS APP。（已可傳輸出來！）

iOS APP學習與撰寫：


（三）已經備有& 需要購買的設備：

已經備有：
 (1) 11個大陸版Arduino (2) 4個義大利版Arduino (3) 7個HM10 (4) 2個ModiaTech (5) 2個pololu (6) 1個Arduino壓克力罩

需要購買：
 (1) 10個Arduino壓克力罩：35／個 http://tw.taobao.com/item/42882555890.htm?spm=a1z3o.7406521.101.54.NodgCU&abtest=_AB-LR492-LR501-LR517-PR492-PR501-PV517_1271
 (2) 九軸-MPU9250：200／個 http://taiwan.tmall.com/item/39998137492.htm?spm=a1z3o.7406521.101.1.Fn0mir&abtest=_AB-LR492-LR501-LR517-PR492-PR501-PV517_1271
 (3) BLE(尚考慮要買HM10 or ModiaTech)：35／個  
(4) 11個行動電源：150／個 http://goods.ruten.com.tw/item/show?21439268772641 
(5) 11小麵包板：15／個 http://taiwan.tmall.com/item/44296520549.htm?id=44296520549&ali_refid=a3_430534_1006:1110430957:N:%E9%9D%A2%E5%8C%85%E6%9D%BF:a2ded3a71571634bb58355e212936ae1&ali_trackid=1_a2ded3a71571634bb58355e212936ae1&spm=a1z3o.7406521.0.0.pWUd9M
(6) USB-AB cable：10/個 http://tw.taobao.com/item/44096894052.htm?spm=a1z3o.7406521.101.31.ppJSWe&abtest=_AB-LR492-LR501-LR517-PR492-PR501-PV517_1271
(7) 腳踏車後輪支架：／個 
(8) 2個電子零件盒：／個
零售五金行
(9) 1組Webduino：1,800／個 http://webduino.io/buy/package-webduino-plus.html
