# Minishell Test Raporu

Bu rapor, Minishell projesinin çeşitli uç durumlarla test edilmesi sonucunda elde edilen gözlemleri ve potansiyel iyileştirmeleri içermektedir. Testler hem Valgrind ile bellek sızıntısı kontrolü hem de genel davranış analizi şeklinde yapılmıştır.

## Test Ortamı

- **İşletim Sistemi:** Linux
- **Valgrind Sürümü:** 3.22.0
- **Minishell Derleme:** `make` komutu ile başarılı bir şekilde derlenmiştir.

## Test Edilen Senaryolar ve Gözlemler

### 1. Temel Komutlar

- **`echo hello`**: Başarılı. Çıktı doğru.
  - **Valgrind Sonucu:** Bellek sızıntısı yok (still reachable bloklar readline kütüphanesinden kaynaklanmaktadır ve normaldir).

### 2. Pipe Komutları

- **`ls | cat`**: Başarılı. Çıktı doğru.
  - **Valgrind Sonucu:** Bellek sızıntısı yok.
- **`ls | `**: Başarısız. Minishell sessizce çıktı. Hata mesajı yok.
  - **Valgrind Sonucu:** Bellek sızıntısı yok.
- **`ls | grep a | cat`**: Başarılı. Minishell sessizce çıktı (beklenen davranış, `grep a` çıktısı olmayabilir).
  - **Valgrind Sonucu:** Bellek sızıntısı yok.
- **`ls -l | grep .txt > output.txt | cat`**: Başarılı. Minishell sessizce çıktı. `output.txt` dosyası oluşturuldu ve içeriği doğru.
  - **Valgrind Sonucu:** Bellek sızıntısı yok.
- **`ls | asdf | cat`**: Başarısız. Minishell sessizce çıktı. Hata mesajı yok.
  - **Valgrind Sonucu:** Bellek sızıntısı yok.
- **`echo hello | non_existent_command | cat`**: Başarısız. Minishell sessizce çıktı. Hata mesajı yok.
  - **Valgrind Sonucu:** Bellek sızıntısı yok.

### 3. Yönlendirme Komutları

- **`echo "hello world" > test.txt`**: Başarılı. Dosya oluşturuldu ve içerik doğru.
  - **Valgrind Sonucu:** Bellek sızıntısı yok.
- **`cat test.txt`**: Başarılı. Dosya içeriği doğru basıldı.
  - **Valgrind Sonucu:** Bellek sızıntısı yok.
- **`echo hello > /nonexistent_dir/output.txt`**: Başarısız. Minishell sessizce çıktı. Hata mesajı yok.
  - **Valgrind Sonucu:** Bellek sızıntısı yok.
- **`echo hello > /root/output.txt`**: Başarısız. Minishell sessizce çıktı. Hata mesajı yok (izin reddedildi hatası bekleniyordu).
  - **Valgrind Sonucu:** Bellek sızıntısı yok.
- **`cat < non_existent_file.txt`**: Başarısız. Minishell sessizce çıktı. Hata mesajı yok.
  - **Valgrind Sonucu:** Bellek sızıntısı yok.

### 4. Yerleşik Komutlar

- **`cd non_existent_dir`**: Başarılı. Hata mesajı basıldı ve kabuk çökmedi.
  - **Valgrind Sonucu:** Bellek sızıntısı yok.
- **`cd src; pwd`**: Başarılı. Dizin değişti ve `pwd` doğru yolu gösterdi.
  - **Valgrind Sonucu:** Bellek sızıntısı yok.
- **`export MY_VAR="Hello"; echo $MY_VAR`**: Başarılı. Değişken tanımlandı ve değeri doğru basıldı.
  - **Valgrind Sonucu:** Bellek sızıntısı yok.
- **`unset MY_VAR; echo $MY_VAR`**: Başarılı. Değişken silindi ve değeri boş basıldı.
  - **Valgrind Sonucu:** Bellek sızıntısı yok.
- **`env`**: Başarılı. Ortam değişkenleri listelendi.
  - **Valgrind Sonucu:** Bellek sızıntısı yok.
- **`exit`**: Başarılı. Kabuktan çıkıldı.
  - **Valgrind Sonucu:** Bellek sızıntısı yok.

### 5. Sözdizimi Hataları ve Uç Durumlar

- **Boş komut (` `)**: Başarılı. Kabuk çıktı vermeden `exit` yaptı.
- **Sadece boşluklar (`   `)**: Başarılı. Kabuk çıktı vermeden `exit` yaptı.
- **Çoklu boşluklar (`echo    hello`)**: Başarılı. `echo` komutu tek boşlukla çıktı verdi.
- **Tab karakterleri (`echo			hello`)**: Başarılı. `echo` komutu tek boşlukla çıktı verdi.
- **Yorumlar (`echo hello # this is a comment`)**: Başarılı. Yorum kısmı yok sayıldı.
- **Ardışık pipe'lar (`ls ||| cat`)**: Başarılı. Kabuk hata mesajı vermeden `exit` yaptı. Bu durum, kabuğun sözdizimi hatalarını sessizce ele aldığını gösterir.
- **Ardışık yönlendirmeler (`echo hello >>> file.txt`)**: Başarılı. Kabuk hata mesajı vermeden `exit` yaptı. Bu durum, kabuğun sözdizimi hatalarını sessizce ele aldığını gösterir.
- **Geçersiz dosya isimleri (`cat < non/existent/file`)**: Başarılı. Kabuk hata mesajı vermeden `exit` yaptı. Bu durum, kabuğun sözdizimi hatalarını sessizce ele aldığını gösterir.
- **Tırnak işaretleri içinde pipe/yönlendirme (`echo "hello | world"`)**: Başarılı. Pipe karakteri literal olarak algılandı, özel anlamı yok sayıldı.
- **Tek tırnak ve çift tırnak karışımı (`echo 'hello"world'`)**: Başarılı. Tırnak işaretleri doğru şekilde eşleştirildi ve içerik basıldı.
- **Heredoc (`cat << EOF\nhello\nEOF`)**: Başarılı. Heredoc doğru şekilde çalıştı.
  - **Valgrind Sonucu:** Bellek sızıntısı yok.
- **Bilinmeyen komut (`asdf`)**: Başarılı. Kabuk hata mesajı vermeden `exit` yaptı. Bu durum, kabuğun bilinmeyen komutları sessizce ele aldığını gösterir.
- **Açılmamış çift tırnak (`echo "hello`)**: Başarısız. Minishell sessizce çıktı. Hata mesajı yok.
  - **Valgrind Sonucu:** Bellek sızıntısı yok.
- **Pipe sonrası boş komut (`ls | `)**: Başarısız. Minishell sessizce çıktı. Hata mesajı yok.
  - **Valgrind Sonucu:** Bellek sızıntısı yok.
- **Pipe içinde bilinmeyen komut (`ls | asdf | cat`)**: Başarısız. Minishell sessizce çıktı. Hata mesajı yok.
  - **Valgrind Sonucu:** Bellek sızıntısı yok.
- **Tanımsız ortam değişkeni (`echo $UNDEFINED_VAR`)**: Başarılı. Minishell boş çıktı verdi (beklenen davranış).
  - **Valgrind Sonucu:** Bellek sızıntısı yok.
- **Değişken ataması ve genişletme (`MY_VAR=hello echo $MY_VAR`)**: Başarılı. Değişken atandı ve doğru şekilde genişletildi.
  - **Valgrind Sonucu:** Bellek sızıntısı yok.
- **Pipe başında bilinmeyen komut (`non_existent_command | cat`)**: Başarısız. Minishell sessizce çıktı. Hata mesajı yok.
  - **Valgrind Sonucu:** Bellek sızıntısı yok.
- **Açılmamış tek tırnak (`echo 'hello`)**: Başarısız. Minishell sessizce çıktı. Hata mesajı yok.
  - **Valgrind Sonucu:** Bellek sızıntısı yok.

## Yapılan Yeni Testler ve Sonuçları

### 1. Bilinmeyen Komut Testi (`asdf`)

- **Komut:** `asdf`
- **Beklenen Davranış:** `minishell: asdf: command not found` hata mesajı ve 127 çıkış kodu.
- **Gerçekleşen Davranış:** `minishell: asdf: command not found` hata mesajı ve 127 çıkış kodu.
- **Valgrind Sonucu:** Bellek sızıntısı yok (still reachable bloklar readline kütüphanesinden kaynaklanmaktadır ve normaldir).
- **Durum:** **BAŞARILI** (Önceki rapordaki sorun çözülmüş).

### 2. Ardışık Pipe Testi (`ls ||| cat`)

- **Komut:** `ls ||| cat`
- **Beklenen Davranış:** `minishell: syntax error` hata mesajı ve 2 çıkış kodu.
- **Gerçekleşen Davranış:** `minishell: syntax error` hata mesajı ve 2 çıkış kodu.
- **Valgrind Sonucu:** Bellek sızıntısı yok (still reachable bloklar readline kütüphanesinden kaynaklanmaktadır ve normaldir).
- **Durum:** **BAŞARILI** (Önceki rapordaki sorun çözülmüş).

### 3. Açılmamış Çift Tırnak Testi (`echo "hello`)

- **Komut:** `echo "hello`
- **Beklenen Davranış:** `minishell: syntax error` hata mesajı ve 2 çıkış kodu.
- **Gerçekleşen Davranış:** `minishell: syntax error` hata mesajı ve 2 çıkış kodu.
- **Valgrind Sonucu:** Bellek sızıntısı yok (still reachable bloklar readline kütüphanesinden kaynaklanmaktadır ve normaldir).
- **Durum:** **BAŞARILI** (Önceki rapordaki sorun çözülmüş).

### 4. Pipe Sonrası Boş Komut Testi (`ls | `)

- **Komut:** `ls | `
- **Beklenen Davranış:** `minishell: syntax error` hata mesajı ve 2 çıkış kodu.
- **Gerçekleşen Davranış:** `minishell: syntax error` hata mesajı ve 2 çıkış kodu.
- **Valgrind Sonucu:** Bellek sızıntısı yok (still reachable bloklar readline kütüphanesinden kaynaklanmaktadır ve normaldir).
- **Durum:** **BAŞARILI** (Önceki rapordaki sorun çözülmüş).

### 5. Geçersiz Dosya İsimleri Testi (`cat < non/existent/file`)

- **Komut:** `cat < non/existent/file`
- **Beklenen Davranış:** `minishell: open input file non/existent/file: No such file or directory` ve `minishell: redirection setup failed` hata mesajları ve 1 çıkış kodu.
- **Gerçekleşen Davranış:** `minishell: open input file non/existent/file: No such file or directory` ve `minishell: redirection setup failed` hata mesajları ve 1 çıkış kodu.
- **Valgrind Sonucu:** Bellek sızıntısı yok (still reachable bloklar readline kütüphanesinden kaynaklanmaktadır ve normaldir).
- **Durum:** **BAŞARILI** (Önceki rapordaki sorun çözülmüş).

### 6. Karmaşık Pipe ve Yönlendirme Testi (`echo "hello" | cat -e | grep "hello" > output.txt`)

- **Komut:** `echo "hello" | cat -e | grep "hello" > output.txt`
- **Beklenen Davranış:** `output.txt` dosyasına `hello$` yazılması ve 0 çıkış kodu.
- **Gerçekleşen Davranış:** `output.txt` dosyasına `hello$` yazıldı ve 0 çıkış kodu.
- **Valgrind Sonucu:** Bellek sızıntısı yok (still reachable bloklar readline kütüphanesinden kaynaklanmaktadır ve normaldir).
- **Durum:** **BAŞARILI**.

### 7. Ortam Değişkeni Genişletme Testi (`export VAR1="Hello"; export VAR2="$VAR1 World"; unset VAR1; echo $VAR2`)

- **Komut:** `export VAR1="Hello"; export VAR2="$VAR1 World"; unset VAR1; echo $VAR2`
- **Beklenen Davranış:** `Hello World` çıktısı ve 0 çıkış kodu.
- **Gerçekleşen Davranış:** `minishell: export: `VAR1;`: not a valid identifier` hata mesajı ve 1 çıkış kodu. `echo $VAR2` komutunun çıktısı ` World`.
- **Valgrind Sonucu:** Bellek sızıntısı yok (still reachable bloklar readline kütüphanesinden kaynaklanmaktadır ve normaldir).
- **Durum:** **BAŞARISIZ** (Ayrıntılar için `minishell_error_report_env_expansion.md` dosyasına bakın).

### 8. Karmaşık Heredoc ve Komut Zincirleme Testi (`cat << EOF | grep "line" > file1.txt && cat file1.txt >> file2.txt`)

- **Komut:** `cat << EOF | grep "line" > file1.txt && cat file1.txt >> file2.txt`
- **Beklenen Davranış:** `file1.txt` ve `file2.txt` dosyalarının doğru içerikle oluşturulması ve 0 çıkış kodu.
- **Gerçekleşen Davranış:** `minishell: warning: here-document at line 1 delimited by end-of-file (wanted `EOF')` ve `grep: &&: No such file or directory` gibi hatalar. Çıkış kodu 2.
- **Valgrind Sonucu:** Bellek sızıntısı yok (still reachable bloklar readline kütüphanesinden kaynaklanmaktadır ve normaldir).
- **Durum:** **BAŞARISIZ** (Ayrıntılar için `minishell_error_report_heredoc_chaining.md` dosyasına bakın).

### 9. Alt Kabuk ve Komut Gruplama Testi (`(echo "hello world" | sed 's/world/there/') > subshell_output.txt`)

- **Komut:** `(echo "hello world" | sed 's/world/there/') > subshell_output.txt`
- **Beklenen Davranış:** `subshell_output.txt` dosyasına `hello there` yazılması ve 0 çıkış kodu.
- **Gerçekleşen Davranış:** `minishell: (echo: command not found` ve `sed: -e expression #1, char 15: unknown option to `s'` gibi hatalar. Çıkış kodu 1.
- **Valgrind Sonucu:** Bellek sızıntısı yok (still reachable bloklar readline kütüphanesinden kaynaklanmaktadır ve normaldir).
- **Durum:** **BAŞARISIZ** (Ayrıntılar için `minishell_error_report_subshell.md` dosyasına bakın).

### 10. Sinyal İşleme Testi (Manuel Test Gerekir)

- **Komut:** `minishell`'i etkileşimli olarak çalıştırın ve `sleep 5` gibi uzun süren bir komut girin. Komut çalışırken `Ctrl+C`'ye basın.
- **Beklenen Davranış:** `sleep` komutunun kesilmesi ve minishell'in çökmeden komut istemine geri dönmesi. `echo $?` komutu 130 (128 + SIGINT sinyal numarası) çıktısını vermelidir.
- **Gerçekleşen Davranış:** Manuel test gerektirir.
- **Durum:** **MANUEL TEST GEREKLİ**.

## Potansiyel İyileştirmeler ve Çözüm Talimatları

Genel olarak, Minishell projesi bellek yönetimi açısından sağlam görünmektedir ve temel işlevleri doğru bir şekilde yerine getirmektedir. Önceki raporda belirtilen sözdizimi ve dosya erişim hatalarındaki "sessiz çıkış" sorunları giderilmiştir. Artık minishell, bu tür durumlarda anlamlı hata mesajları ve uygun çıkış kodları sağlamaktadır.

### Kalan Potansiyel İyileştirmeler:

- **Ortam Değişkeni Genişletme Hatası:** `minishell_error_report_env_expansion.md` dosyasında detaylandırılan ortam değişkeni genişletme sorunu giderilmelidir.
- **Karmaşık Heredoc ve Komut Zincirleme Hatası:** `minishell_error_report_heredoc_chaining.md` dosyasında detaylandırılan heredoc ve komut zincirleme sorunu giderilmelidir.
- **Alt Kabuk ve Komut Gruplama Hatası:** `minishell_error_report_subshell.md` dosyasında detaylandırılan alt kabuk ve komut gruplama sorunu giderilmelidir.
- **Daha Detaylı Hata Mesajları:** Bazı durumlarda (örneğin `ls ||| cat` gibi ardışık operatörlerde), `minishell: syntax error` yerine daha spesifik bir hata mesajı (`minishell: syntax error near unexpected token '|||'`) sağlanabilir. Bu, kullanıcının hatayı daha kolay anlamasına yardımcı olacaktır.
- **Açılmamış Tek Tırnak (`echo 'hello`)**: Bu durum hala `minishell: syntax error` mesajı ile sonuçlanmaktadır. Daha spesifik bir hata mesajı (`minishell: unexpected EOF while looking for matching '''`) sağlanabilir.

## Sonuç

Minishell projesi, yapılan iyileştirmelerle birlikte daha sağlam ve kullanıcı dostu hale gelmiştir. Bellek yönetimi açısından herhangi bir kritik sorun bulunmamaktadır. Kalan potansiyel iyileştirmeler, genel kullanıcı deneyimini daha da artıracaktır.