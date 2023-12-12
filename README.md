# ccLemon

仙台高専4年時のネットワークプログラミングの授業で作成した 「CCレモンゲーム」 です。サーバーを介して対戦型で遊べるようになっています。  

## 使い方

```bash
cd ccLemon
gcc -o cclemon main.c
./cclemon
```

コマンドは数字で入力を行います。

| コマンド | コード | コスト |  
| -- | -- | -- |  
| チャージ | 0 | +1 |  
| ブロック | 1 | 0 |  
| アタック | 2 | -1 |  
| キロアタック | 3 | -3 |  

ルールを簡単に説明すると以下の通りです。  
- チャージを行うとコストが1増加する
- ブロックはコストが発生せず、制限なく利用できる
- アタックを行うためにはコストを1消費する
- キロアタックを行うためにはコストを3消費する
- アタックはブロックで防ぐことが可能
- キロアタックはブロックで防ぐことはできない
- チャージ中にアタックまたはフルアタックを受けると敗北する
- 双方のプレイヤーがアタックまたはキロアタックを行った場合は相殺され、ゲームは続行する

