# Minishell Hata Raporu - Ortam Değişkeni Genişletme

## Sorun Tanımı

Minishell, ortam değişkeni genişletme sırasında noktalı virgül (`;`) içeren komutlarda hatalı davranış sergilemektedir. Özellikle `export VAR2="$VAR1 World";` gibi bir komut verildiğinde, `$VAR1` değişkeninin değeri doğru bir şekilde genişletilmemekte ve `export` komutu noktalı virgülü değişken adının bir parçası olarak algılayarak `minishell: export: `VAR1;`: not a valid identifier` şeklinde bir hata mesajı döndürmektedir. Ayrıca, `echo $VAR2` komutunun çıktısı da beklenen `Hello World` yerine ` World` olarak gelmektedir.

## Kök Neden

Bu sorun, minishell'in komut ayrıştırma (parsing) ve/veya değişken genişletme (expansion) aşamalarında noktalı virgül gibi özel karakterlerin tırnak işaretleri içinde doğru bir şekilde işlenmesinden kaynaklanmaktadır. Tırnak işaretleri içindeki karakterlerin literal olarak algılanması gerekirken, noktalı virgülün komut ayırıcı olarak yorumlanması bu hataya yol açmaktadır.

## Çözüm Talimatları

1.  **Lexer/Parser İyileştirmesi:**
    -   Tırnak işaretleri (`'` ve `"`) içindeki karakterlerin literal olarak kabul edildiğinden ve komut ayırıcılar (`;`, `|`, `>`, `<`) olarak yorumlanmadığından emin olun.
    -   Değişken genişletme (`$VAR`) sırasında, değişkenin değerinin doğru bir şekilde alınarak tırnak işaretleri içindeki diğer metinle birleştirildiğinden emin olun.
2.  **Test Senaryosu:**
    -   `export VAR1="Hello"; export VAR2="$VAR1 World"; unset VAR1; echo $VAR2` komutunu çalıştırarak beklenen çıktının `Hello World` olduğundan emin olun.
    -   Valgrind ile bellek sızıntısı kontrolü yapın.

## Sonuç

Bu hata, minishell'in komut ayrıştırma ve değişken genişletme mekanizmalarında önemli bir boşluğu işaret etmektedir. Bu sorunun giderilmesi, kabuğun daha sağlam ve beklenen POSIX davranışına uygun hale gelmesini sağlayacaktır.