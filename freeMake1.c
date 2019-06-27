typedef volatile unsigned short hword;
#define BGR(r,g,b)  ((b<<10)+(g<<5)+r)
#define VRAM 0x06000000;

void move(hword);//操作キャラの動き
void delete_point(hword,hword);//色を消す

void make(int,int,int,int,int,hword);//移動する縦線、横線
void draw_line(int , int , int , int, int,hword);//消えたり現れたり斜め線の作成
void draw_point(hword,hword,hword);//ドットに色を付ける
void draw_stage(int,hword,hword);//スタート画面の作成
void make_Circle(int,int,int,hword);//動く斜めの円の作成
int check(int,int,hword);//操作キャラの当たり判定
void ending(int,hword);//終了画面の作成
int start();//スタート画面からゲーム開始
int skill();//「A」を押すと弾幕のスピードが遅くなる。弾幕が一時的に複製される。

int p_x[3]={};//プレイヤーのX座標3ドット
int p_y[3]={};//プレイヤーのY座標3ドット
int bullet_x[2]={0,0};//円のｘ座標
int bullet_y[2]={0,0};//円のy座標
int count;//縦向きの一つの弾幕の作成
int count1;//横向きの一つの弾幕の作成
int e_move_x;//縦向きの一つの弾幕の横移動
int e_move_y;//横向きの一つの弾幕の縦移動
int line_x;//斜め線のX座標への加算
int line_y;//斜め線のy座標への加算

int main (void){
  
   hword *ptr;//描画モードの設定に必要
   hword color[5];//色の配列の宣言
   int i;//for文用
   int j;//for文用
   int R; //作成される円の半径
   int start1;//「START」が押されたかどうかの判定用
   int end1;//ステージ１のクリア判定用
   int end2;//ステージ２のクリア判定用
   int badend1;//ステージ１のゲームオーバー判定用
   int badend2;//ステージ2のゲームオーバー判定用
   int amount;//while文のループ回数
   int count2;//while分のループ回数かつ「A」の機能の実装用
   int change;//斜めの線分が現れる、あるいは消えるタイミングの調整
   int speed;//「A」を押した際に、どのくらい遅くなるか
   
   //GBAの描画モード設定
   ptr = (hword*) 0x04000000;
   *ptr = 0x0F03;
   
   //様残な種類の色
   color[0] = 0x7FE0;//水色
   color[1] = 0x001F;//赤色
   color[2] = 0x7FFF;//白色
   color[3] = 0x03E0;//緑色
   color[4] = 0x03FF;//黄色
   color[5] = 0x7C1F;//マゼンタ

   ptr = (hword*) VRAM;
   *ptr = color[0];
   R=6;

START://ジャンプ命令があった際に戻ってこれるようにするためのラベル。
    //ローカル変数の初期化
    end1=0;
    end2=0;
    badend1=0;
    badend2=0;
    amount=0;
    count2=1;
    change=0;
    //グローバル変数の初期化
    bullet_x[0]=0;
    bullet_x[1]=0;
    bullet_y[0]=0;
    bullet_y[1]=0;
    count=0;
    count1=0;
    e_move_x=0;
    e_move_y=0;
    line_x=0;
    line_y=0;

    draw_stage(1,color[0],color[2]);
    while(start1!=1){//スタート画面が押されるまで無限ループ
        start1=start();
    }
    for(i=0;i<3;i++){//プレイヤーの座標決定
            p_x[i]=130+i;
            p_y[i]=87+i;
    }
    start1=0;

    while(end1!=1&&badend1!=1){//ステージ1一定時間生き残るか弾幕に触れるまで、ステージ1を無限ループ
        for(i=0;i<8000;i++){//全体の速度を遅くするため
            j+=1;
        }
        amount++;
        speed=skill();
        move(color[3]);
        make(40,20,amount,count2,speed,color[1]);
        if(amount%15==0){//次にmakeを仕様するので、もう一度上のmakeを使用する際に、2つ分一気に飛ばないようにするため
                e_move_x--;
                e_move_y--;
        }//このif文により
        make(220,140,amount,count2,speed,color[4]);
        make_Circle(R,count2,speed,color[2]);
        draw_line(0,180,0,120,change,color[0]);
        draw_line(60,240,40,160,change,color[5]);
        
        badend1=check(p_x[0],p_y[0],color[3]);
        count2++;
        if(line_x==240||line_y==160){//斜め線を構成する弾幕のx座標、y座標が液晶画面の端に行くと元の位置に戻るようにする。
            line_x=0;line_y=0;
            change++;//line_xかline_yの液晶画面の端にたどり着く回数をカウントしている。
        }
        if(amount==1500){//whileが1500回ループすれば、ステージ1クリア
                end1=1;
        }
    }
    if(badend1==1){//敵に当たった時の処理
    //全画面を真っ暗に
        for(i=0;i<240;i++){
          for(j=0;j<160;j++){
              draw_point(i,j,0x0000);
          }
      }
        while(1){//「START」が押されるまで無限ループ
          start1=start();
          if(start1==1){start1=0;goto START;}//「START」が押されればSTARTのラベルまでジャンプする。
        }
    }

    //ステージ1とほぼ同様の流れ
    draw_stage(2,color[0],color[2]);
    while(start1!=1){
        start1=start();
    }
    for(i=0;i<3;i++){
            p_x[i]=130+i;
            p_y[i]=87+i;
    }
     amount=0;
     line_x=0;line_y=0;count2=0;change=0;

     while(end2!=1&&badend2!=1){
        for(i=0;i<4000;i++){//ステージ1と比べて移動速度が速くなる。
            j+=1;
        }
        amount++;
        speed=skill();
        move(color[3]);
        make(40,20,amount,count2,speed,color[1]);
        if(amount%15==0){
                e_move_x--;
                e_move_y--;
        }
        make(220,140,amount,count2,speed,color[4]);
        if(amount%15==0){
                e_move_x--;
                e_move_y--;
        }
        make(50,70,amount,count2,speed ,color[4]);
        make_Circle(R,count2,speed,color[2]);
        draw_line(0,180,0,120,change,color[0]);
        draw_line(60,240,40,160,change,color[5]);
        
        badend2=check(p_x[0],p_y[0],color[3]);
        count2++;
        if(line_x==240||line_y==160){
            line_x=0;line_y=0;
            change++;
        }
        if(amount==4000){//ゲームクリアに必要な時間がステージ1と比べて増加
                end2=1;
        }
    }
    if(badend2==1){
        for(i=0;i<240;i++){
          for(j=0;j<160;j++){
              draw_point(i,j,0x0000);
          }
        }
      while(1){
        start1=sta文t();
        if(start1==1){start1=0;goto START;}
       }
    }
    //ゲームクリア画面の作成
    for(i=0;i<240;i++){
          for(j=0;j<160;j++){
              draw_point(i,j,color[4]);//全画面を黄色に
          }
    }
    ending(60,color[1]);
    ending(40,color[4]);

    while(1){
        start1=start();
        if(start1==1){start1=0;goto START;}
    }
    return 0;//プログラムの終了
}



void draw_point (hword x, hword y, hword color){
     hword *ptr;//ポインタ
     ptr = (hword*) VRAM;//液晶画面の一番左上
     ptr += (y*240+x);//x座標、y座標に当たるメモリアドレス
    *ptr = color;//そのアドレスに色の値を代入
}
void draw_stage(int num,hword color,hword color1){
      int i;//for文用
      int j;//for文用
      for(i=0;i<240;i++){
          for(j=0;j<160;j++){
              draw_point(i,j,color);//全画面をcolorの色にアドレス
          }
      }
      if(num==1){//ステージ1のスタート画面
          for(i=80;i<120;i++){
              draw_point(i,80,color1);//color1の色で一の文字を表示
          }
      }else{//ステージ2のスタート画面
          for(i=80;i<120;i++){
              draw_point(i,80,color1);//color1の色で一の文字を表示
          }
          for(i=60;i<140;i++){
              draw_point(i,100,color1);//これで、画面にcolor1の二の文字が表示
          }
      }
}
void delete_point(hword x,hword y){//draw_pointとほぼ同じ,しかし、指定の場所に黒色の表示
    hword *ptr;
    ptr=(hword*) VRAM;
    ptr=ptr+(y*240+x);
    *ptr= 0x0000;//そのアドレスに黒色の値を代入
}
void move(hword color){

    hword down=0x0080;//下矢印の検査
    hword up=0x0040;//上矢印の検査
    hword left=0x0020;//左矢印の検査
    hword right=0x0010;//右矢印の検査
    hword *enter;//キーを押したときの値
    enter=(hword*)0x04000130;//キーを押したときに値が保存されるアドレス
    int i=0;//for文用
    int j=0;//for文用

        for(i=0;i<3;i++){
            for(j=0;j<3;j++){
           delete_point(p_x[i],p_y[j]);//操作キャラの現在位置を黒色にする
            }
        }

        if(((*enter&down)==0)&&p_y[0]<157){//押したキーが下矢印、かつ今の場所が画面の下端より上
            for(i=0;i<3;i++){
                p_y[i]+=3;//操作キャラの現在位置を3ドット分下にずらす
            }
        }else if(((*enter&up)==0)&&p_y[0]>0){//押したキーが上矢印、かつ今の場所が画面の上端より下
            for(i=0;i<3;i++){
                p_y[i]-=3;//操作キャラの現在位置を3ドット分上にずらす
            }
        }else if(((*enter&left)==0)&&p_x[0]>2){//押したキーが左矢印、かつ今の場所が画面の左端より右
            for(i=0;i<3;i++){
                p_x[i]-=3;//操作キャラの現在位置を3ドット分左にずらす
            }
        }else if(((*enter&right)==0)&&p_x[0]<236){//押したキーが右矢印、かつ今の場所が画面の右端より左
            for(i=0;i<3;i++){
                p_x[i]+=3;//操作キャラの現在位置を3ドット分右にずらす
            }
        }

        for(i=0;i<3;i++){
            for(j=0;j<3;j++){
           draw_point(p_x[i],p_y[j],color);//操作キャラの現在位置にcolorを表示
        }
    }
}
void make(int x,int y,int amount,int count2,int speed,hword color){
    int i=0;
    if(speed==20){//「A」を押したとき
        if(count2%20==0){//この関数が20の倍数回呼び出されたとき

            if(amount%15==0){e_move_x++;e_move_y++;}//この関数が15の倍数回呼び出されたとき

            if(count<152){//生成される弾が画面の右端を超えていない場合
            for(i=0;i<8;i++){delete_point(x+e_move_x-1,count+i);}//一行上の弾を削除
            count+=7;
            for(i=0;i<8;i++){draw_point(x+e_move_x,count+i,color);}//Colorの色の、長さ8ドット分の弾を生成
            }else{count=0;}////生成される弾が画面の右端を超えていると、弾の位置を画面の左端に設定
        //上記の横バージョン
            if(count1<232){
            for(i=0;i<8;i++){delete_point(count1+i,y+e_move_y);}
            count1+=7;
            for(i=0;i<8;i++){draw_point(count1+i,y+e_move_y,color);}
           }else{count1=0;}
            //移動する前の残っている、弾を全て削除
            for(i=0;i<240;i++){delete_point(i,y+e_move_y-1);}
            for(i=0;i<160;i++){delete_point(x+e_move_x-1,i);}
        }   
    }else{//「A」を押してない時
        //押した場合と同様の処理
        if(amount%15==0){e_move_x++;e_move_y++;}
        
        if(count<152){
            for(i=0;i<8;i++){delete_point(x+e_move_x,count+i);}
            count+=7;
            for(i=0;i<8;i++){draw_point(x+e_move_x,count+i,color);}
            }else{count=0;}

        if(count1<232){
            for(i=0;i<8;i++){delete_point(count1+i,y+e_move_y);}
            count1+=7;
            for(i=0;i<8;i++){draw_point(count1+i,y+e_move_y,color);}
           }else{count1

            for(i=0;i<240;i++){delete_point(i,y+e_move_y-1);}
            for(i=0;i<160;i++){delete_point(x+e_move_x-1,i);}
    } 
}
void draw_line(int x1, int x2, int y1, int y2,int change,hword color){
    int	dx, dy;//x軸、y軸の変化
    int	e=0;//xがどのくらい進むとyが1変化するのか
    line_x++;
    dx = x2 - x1;
    dy = y2 - y1;
        if(change%2==0){draw_point(x1 + line_x, y2 -line_y, color);}//changeの値が２の倍数回になると、線分を構成する1ドットを生成する。
        else if(change%5==0){delete_point(x1+line_x,y2-line_y);}//changeの値が5の倍数回になると、線分を構成する1ドットを削除する。
        e += dy;
        if (2*e >= dx) {//yが1変化できるかの確認
        line_y++;
	    e -= dx;
	    }  
}
void make_Circle(int R,int count2,int speed,hword color){
    int i;//for文用
    int j;//for文用
        if(speed==20){//「A」を押したとき
            if(count2%20==0){//この関数が20回呼び出されたとき
                for(i=0;i<=2*R;i++){
                    for(j=0;j<=R;j++){//今の円のｙ地点からの大きさ
                        int height=R*R-((R-i))*((R-i));
                        if(height>=j*j){//あるx軸地点における円の高さより、jが小さい値である場合
                            delete_point(i+bullet_x[0]-R,bullet_y[0]+j);//円のy地点にjを加算した位置の色を削除。結果的に移動する前の円のy地点から下の半円を削除
                            delete_point(i+bullet_x[1]-R,bullet_y[1]-j);//円のy地点にjを減算した位置の色を削除。移動する前の円のy地点から上の半円を削除
                        }
                    }
                }
                if(bullet_x[0]<134){//円のy座標が画面の下端を超えていないのなら
                    //円のx座標とy座標を右にドット分ずらす。
                    bullet_x[0]+=6;
                    bullet_x[1]+=6;
                    bullet_y[0]+=6;
                    bullet_y[1]+=6;
                }else{//円のy座標が画面の下端を超えているなら
                     //円のx座標とy座標を0に戻す。
                    bullet_x[0]=0;
                    bullet_x[1]=0;
                    bullet_y[0]=0;
                    bullet_y[1]=0;
                }
                //上記の円の削除する処理と同様、円を削除する代わりに円を生み出している。
                for(i=0;i<=2*R;i++){
                    for(j=0;j<=R;j++){
                        int height=R*R-((R-i))*((R-i));
                        if(height>=j*j){
                            draw_point(i+bullet_x[0]-R,bullet_y[0]+j,color);
                            draw_point(i+bullet_x[1]-R,bullet_y[1]-j,color);
                        }
                    }
                }         
            }
        }else{//「A」が押されていない時
            //押された場合と同様の処理
            for(i=0;i<=2*R;i++){
                for(j=0;j<=R;j++){
                    int height=R*R-((R-i))*((R-i));
                    if(height>=j*j){
                        delete_point(i+bullet_x[0]-R,bullet_y[0]+j);
                        delete_point(i+bullet_x[1]-R,bullet_y[1]-j);
                    }
                }
            }
            if(bullet_x[0]<136){
                bullet_x[0]+=6;
                bullet_x[1]+=6;
                bullet_y[0]+=6;
                bullet_y[1]+=6;
            }else{
                bullet_x[0]=0;
                bullet_x[1]=0;
                bullet_y[0]=0;
                bullet_y[1]=0;
            }
            for(i=0;i<=2*R;i++){
                for(j=0;j<=R;j++){
                    int height=R*R-((R-i))*((R-i));
                    if(height>=j*j){
                        draw_point(i+bullet_x[0]-R,bullet_y[0]+j,color);
                        draw_point(i+bullet_x[1]-R,bullet_y[1]-j,color);
                    }
                }
            }        
        }
}

int check(int x,int y,hword color){
     hword *ptr;//操作キャラ用
     int end=0;//戻り値の変数
     ptr += (y*240+x);//操作キャラのアドレスの取得
     if((*ptr != color)){//操作キャラの左端のドットの色がcolor出ない場合
         end=1;
     }
    return end;
}
int start(){
    hword st=0x0008;//「START」の判定用
    hword *enter;//実際にキーが押されたときのデータ
    hword *ptr;//画面削除用
    enter=(hword*)0x04000130;//実際にキーが押されたとき、データが保存されるアドレス
    int i=0;//for文用
    int start=0;//戻り値の変数
    ptr=(hword*) VRAM;//画面の一番上の左端のアドレス
    if(((*enter&st)==0)){//「START」が押された場合
       start=1;
        for(i=0;i<=240*160;i++){//全画面を黒色に
            ptr=ptr+1;
            *ptr=0x0000;
        }
    }
    return start;
}
int skill(){
    hword a=0x0001;//「A」が押されかの判定用
    hword *enter;//実際にキーが押されたときのデータ
    enter=(hword*)0x04000130;//実際にキーが押されたとき、データが保存されるアドレス
    int speed=1;//戻りの値の変数
    if(((*enter&a)==0)){//「A」が押されたとき
       speed=20;
    }
    return speed;
}
void ending(int R,hword color){
    int i;//for文用
    int j;//for文用
    //make_Circleで使用された、円の作成のプログラムと同じ
    for(i=0;i<=2*R;i++){
        for(j=0;j<=R;j++){
            int height=R*R-((R-i))*((R-i));
            if(height>=j*j){
                draw_point(i+120-R,80+j,color);
                draw_point(i+120-R,80-j,color);
            }
        }
    }   
}