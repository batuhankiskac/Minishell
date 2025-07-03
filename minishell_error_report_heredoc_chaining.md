# Minishell Hata Raporu - Karmaşık Heredoc ve Komut Zincirleme

## Sorun Tanımı

Minishell, heredoc kullanımı ve komut zincirleme (`&&`) operatörünün bir arada kullanıldığı karmaşık senaryolarda hatalı davranış sergilemektedir. Özellikle aşağıdaki gibi bir komut dizisi verildiğinde:

`cat << EOF | grep "line" > file1.txt && cat file1.txt >> file2.txt`

ve heredoc içeriği:
```
first line
second line
third line
EOF
```

Minishell, heredoc'u doğru bir şekilde sonlandıramamakta (`minishell: warning: here-document at line 1 delimited by end-of-file (wanted `EOF')`) ve `&&` operatörünü bir komut ayırıcı olarak değil, `grep` komutuna bir dosya argümanı olarak iletmektedir (`grep: &&: No such file or directory`). Ayrıca, heredoc içeriği de `grep` komutuna yanlışlıkla dosya argümanı olarak geçmektedir.

## Kök Neden

Bu sorun, minishell'in komut ayrıştırma (parsing) aşamasında birden fazla nedenden kaynaklanmaktadır:

1.  **Heredoc Ayrıştırma Hatası:** Heredoc sonlandırma işaretçisinin (`EOF`) doğru bir şekilde algılanamaması ve heredoc içeriğinin komutun bir parçası olarak yorumlanması.
2.  **Operatör Ayrıştırma Hatası:** `&&` gibi kontrol operatörlerinin, pipe (`|`) ve yönlendirmeler (`>`, `>>`) ile birlikte kullanıldığında doğru öncelik ve ayrıştırma kurallarına göre işlenmemesi.
3.  **Argüman Geçiş Hatası:** Heredoc içeriğinin ve kontrol operatörlerinin, pipe'ın sağ tarafındaki komutlara (örneğin `grep`) yanlışlıkla argüman olarak geçirilmesi.

## Çözüm Talimatları

1.  **Heredoc Ayrıştırma Düzeltmesi:**
    -   Heredoc içeriğinin başlangıcını ve bitişini doğru bir şekilde belirlemek için lexer/parser mantığını gözden geçirin. Heredoc içeriğinin, sonlandırma işaretçisiyle biten ayrı bir blok olarak ele alındığından emin olun.
    -   Heredoc içeriğinin, pipe aracılığıyla yalnızca ilgili komuta (bu durumda `cat`) iletildiğinden ve diğer komutlara (örneğin `grep`) argüman olarak geçmediğinden emin olun.
2.  **Operatör Önceliği ve Zincirleme Düzeltmesi:**
    -   `|`, `>`, `>>`, `&&` gibi operatörlerin POSIX standartlarına uygun öncelik ve ilişkilendirme kurallarına göre ayrıştırıldığından emin olun.
    -   Komut zincirleme operatörlerinin (`&&`, `||`) ayrı komut bloklarını doğru bir şekilde ayırdığından ve bir sonraki komutun yalnızca önceki komutun başarı durumuna göre yürütüldüğünden emin olun.
3.  **Argüman Temizliği:**
    -   Komutlara iletilen argümanların, yalnızca ilgili komutun beklediği argümanlar olduğundan ve ayrıştırma sırasında oluşan artıklardan arındırıldığından emin olun.

## Sonuç

Bu hata, minishell'in komut ayrıştırma ve yürütme motorunda temel bir sorunu işaret etmektedir. Bu sorunun giderilmesi, kabuğun karmaşık komut dizilerini doğru bir şekilde işlemesini ve beklenen kabuk davranışını sergilemesini sağlayacaktır.