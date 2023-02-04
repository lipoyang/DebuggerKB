DebuggerKB
=========
## 概要
IDEでのデバッグ関連のショートカットキーを並べた専用キーボードです。

* 有線接続(USB HID)と無線接続(BLE HID)の両方に対応。
* LiPoバッテリー(400mAh)と充電回路を内蔵。
* キーマップは最大8パターンまで登録でき、切替キーでキーマップを切り替え可能。
* キーマップはUSB接続時にブラウザアプリから設定可能。<br>(Chrome や Edge など Web Serial API に対応したブラウザが必要)
* Nordic nRF52840 搭載。 (Seeed Studio XIAO nRF52840)
* Arduinoベースの独自ファームウェア。 (QMKではありません。)

## ファイル一覧

* hard/
    * DebuggerKB.sch/brd : 回路/基板設計データ(Eagle形式)
    * DebuggerKB.pdf : 回路図(PDF)
    * DebuggerKB.zip : ガーバーデータの圧縮ファイル
    * DebuggerKB_BOM.xlsx : BOMリスト(Excel形式)
* mecha/
    * A5_DebuggerKB.svg : プレート(アクリル板)の加工データ
    * DSA_1u_4x4_DebuggerKB.svg : キーキャップのUV印刷データ
* firm/DebuggerKB/
    * DebuggerKB.ino : キーボードのファームウェア (Arduinoスケッチ)
    * \*.h, \*.cpp: ファームウェアで使用する各種クラス宣言と実装
* app/DebuggerKB_Config/ 
    * index.html : キーマップ設定アプリ (ブラウザアプリ)
    * \*.js, \*.png, css/\*, lib/\* : キーマップ設定アプリで使用する各種ファイル



