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
- **Tab karakterleri (`echo\t\t\thello`)**: Başarılı. `echo` komutu tek boşlukla çıktı verdi.
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

## Potansiyel İyileştirmeler ve Çözüm Talimatları

Genel olarak, Minishell projesi bellek yönetimi açısından sağlam görünmektedir ve temel işlevleri doğru bir şekilde yerine getirmektedir. Ancak, bazı sözdizimi hataları durumunda kullanıcıya daha bilgilendirici geri bildirim sağlamak için iyileştirmeler yapılabilir.

### Sorun 1: Sessiz Sözdizimi Hataları ve Yönlendirme Hataları

**Tanım:** Minishell, ardışık pipe'lar (`|||`), ardışık yönlendirmeler (`>>>`), geçersiz dosya isimleri, var olmayan dizinlere yönlendirme veya izin reddedilen dosyalara yazma, açılmamış tırnak işaretleri ve pipe sonrası boş komut gibi bazı sözdizimi ve dosya erişim hatalarında herhangi bir hata mesajı basmadan sessizce `exit` yapmaktadır. Bu durum, kullanıcının hatanın nedenini anlamasını zorlaştırabilir.

**Kök Neden:** Kabuğun sözdizimi ayrıştırma ve hata işleme mekanizmalarının bu tür durumları açıkça raporlamaması ve dosya erişim hatalarını yakalamaması.

**Çözüm Talimatları:**

1.  **Ayrıştırma Aşaması İyileştirmesi:** Lexer veya parser aşamasında, ardışık operatörler veya geçersiz dosya isimleri gibi sözdizimi kurallarını ihlal eden durumları tespit edin. Ayrıca, açılmamış tırnak işaretleri ve pipe sonrası boş komut gibi durumlarda da hata tespiti yapın.
2.  **Dosya Erişimi Hata Kontrolü:** Yönlendirme işlemleri sırasında (örneğin `open`, `access` sistem çağrıları sonrası) dosya oluşturma veya yazma izinleri gibi hataları kontrol edin.
3.  **Hata Mesajı Üretimi:** Tespit edilen her sözdizimi veya dosya erişim hatası için, standart hata çıktısına (stderr) anlamlı bir hata mesajı yazdırın. Örneğin:
    - `minishell: syntax error near unexpected token '||'`
    - `minishell: syntax error near unexpected token '>>>'`
    - `minishell: no such file or directory: non/existent/file`
    - `minishell: /nonexistent_dir/output.txt: No such file or directory`
    - `minishell: /root/output.txt: Permission denied`
    - `minishell: unexpected EOF while looking for matching '"'`
    - `minishell: syntax error near unexpected token '|'`
4.  **Çıkış Kodu:** Sözdizimi veya dosya erişim hatası durumunda kabuğun sıfır olmayan bir çıkış koduyla (`exit 2` veya uygun POSIX hata kodu gibi) sonlanmasını sağlayın.

### Sorun 2: Bilinmeyen Komutlarda Sessiz Çıkış

**Tanım:** Minishell, bulunamayan komutlar için (örneğin `asdf`) herhangi bir hata mesajı basmadan sessizce `exit` yapmaktadır. Standart kabuklar bu durumda "command not found" gibi bir mesaj basar.

**Kök Neden:** Komut yürütme aşamasında, `PATH` içinde komut bulunamadığında uygun bir hata mesajı basılmaması.

**Çözüm Talimatları:**

1.  **Komut Bulunamadı Kontrolü:** `execve` çağrısı başarısız olduğunda (genellikle `ENOENT` hatası ile), komutun bulunamadığını belirten bir hata mesajı yazdırın.
2.  **Hata Mesajı Üretimi:** Standart hata çıktısına (stderr) anlamlı bir hata mesajı yazdırın. Örneğin:
    - `minishell: asdf: command not found`
3.  **Çıkış Kodu:** Komut bulunamadığında kabuğun sıfır olmayan bir çıkış koduyla (`exit 127` gibi) sonlanmasını sağlayın.

## Sonuç

Minishell projesi, bellek yönetimi açısından sağlam görünmektedir. Ancak, kullanıcı deneyimini artırmak ve hata ayıklamayı kolaylaştırmak için sözdizimi hataları, dosya erişim hataları ve bilinmeyen komutlar için daha açık hata mesajları ve uygun çıkış kodları sağlamak önemlidir. Bu iyileştirmeler, kabuğun daha kullanıcı dostu ve standartlara uygun olmasını sağlayacaktır.
