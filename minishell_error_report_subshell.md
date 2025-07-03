# Minishell Hata Raporu - Alt Kabuk ve Komut Gruplama

## Sorun Tanımı

Minishell, alt kabuklar ve komut gruplama (`()`) kullanıldığında hatalı davranış sergilemektedir. Özellikle aşağıdaki gibi iç içe komutlar içeren bir senaryoda:

`(echo "hello world" | sed 's/world/there/') > subshell_output.txt`

Minishell, parantez içindeki komutu (`(echo`) tek bir komut olarak algılamakta ve `minishell: (echo: command not found` hatası vermektedir. Ayrıca, `sed` komutuna yanlış argümanlar iletilmekte ve `sed: -e expression #1, char 15: unknown option to `s'` hatası oluşmaktadır. Bu durum, alt kabuk içindeki pipe ve yönlendirmelerin de doğru işlenmediğini göstermektedir.

## Kök Neden

Bu sorun, minishell'in komut ayrıştırma (parsing) aşamasında parantezlerin ve içindeki komut gruplarının doğru bir şekilde yorumlanmamasından kaynaklanmaktadır. Minishell, `(` karakterini özel bir operatör olarak tanımamakta ve içindeki ifadeyi tek bir komut adı olarak değerlendirmektedir.

## Çözüm Talimatları

1.  **Alt Kabuk Ayrıştırma Düzeltmesi:**
    -   Lexer ve parser'ı, parantezleri (`(` ve `)`) özel operatörler olarak tanıyacak ve içindeki ifadeyi ayrı bir komut bloğu veya alt kabuk olarak işleyecek şekilde güncelleyin.
    -   Alt kabuk içindeki pipe ve yönlendirmelerin de doğru bir şekilde ayrıştırıldığından ve yürütüldüğünden emin olun.
2.  **Komut Yürütme Düzeltmesi:**
    -   Alt kabuk içindeki komutların, ana kabuktan bağımsız bir ortamda doğru bir şekilde yürütüldüğünden emin olun.
    -   Pipe ve yönlendirmelerin, alt kabuk içinde de beklenen şekilde çalıştığından emin olun.

## Sonuç

Bu hata, minishell'in temel komut ayrıştırma ve yürütme yeteneklerinde önemli bir eksikliği işaret etmektedir. Alt kabukların doğru bir şekilde desteklenmesi, kabuğun daha karmaşık ve esnek komut dizilerini işlemesi için kritik öneme sahiptir.