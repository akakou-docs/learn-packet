# let-us-make-packets
パケットを作るぞい（ ＾ω＾）

## client

### ビルド
`./client/bin` と `./server/bin` を作成し、 

`./client` または `./server` に移動してから
> make

でビルドされます。

#### 例
```sh
$ cd let-us-make-packets/client/
$ make
```

### 実行
実行ファイルは`./client/bin` または `./server/bin`にあります。

```sh
# ./実行ファイル ネットワークインターフェース名
```

#### 例
```sh
$ sudo /実行ファイル enp0s25
```
