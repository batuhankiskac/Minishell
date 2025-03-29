### 1. Komut Yapısının Alınması ve Karar Verme
- **Parsed Command:**
  Parser tarafından üretilen, komut adını, argümanları, redirection bilgilerini (giriş/çıkış yönlendirmesi, append, heredoc vs.) ve pipe operatörlerini içeren bir yapıyı kabul et.

- **Built-in Kontrolü:**
  Öncelikle, komutun built-in (ör. cd, export, unset, env, exit, pwd, echo) olup olmadığını kontrol et. Eğer built-in ise, ilgili built-in fonksiyonu çağır ve işlemi tamamla.

---

### 2. Dış Komutların Çalıştırılması
- **Komut Arama ve PATH:**
  - Komut, eğer built-in değilse, PATH environment değişkeni üzerinden aranmalı.
  - Komutun tam yolunu bulmak için PATH içindeki dizinlerde arama yap ve doğru yürütülebilir dosya yolunu belirle.

- **Fork ve Execve İşlemleri:**
  - Yeni bir süreç oluşturmak için `fork()` kullan.
  - Çocuk süreçte, `execve()` çağrısı yaparak komutu yürüt.
  - Ebeveyn süreç, çocuk sürecin bitmesini `waitpid()` ile bekle.

- **Environment'ın Geçişi:**
  - Dış komutları çalıştırmadan önce, environment bağlı listesi (t_env) char ** formatına dönüştürülmeli ve execve çağrısına aktarılmalıdır.

---

### 3. Redirection (Yönlendirme) İşlemleri
- **Giriş (Input) Yönlendirmesi:**
  - Komut satırında `<` operatörü kullanıldığında, ilgili dosya açılır ve standart giriş (stdin) bu dosyaya yönlendirilir.

- **Çıkış (Output) Yönlendirmesi:**
  - `>` operatörü kullanıldığında, dosya açılır (veya oluşturulur) ve standart çıkış (stdout) bu dosyaya yönlendirilir.
  - `>>` operatörü ile çıktı ekleme modunda yönlendirilir.

- **Heredoc (<<):**
  - Belirtilen delimiter'a kadar okuma yapılır ve geçici bir dosya veya pipe aracılığıyla komutun girişine aktarılır.

---

### 4. Pipe İşlemleri
- **Pipeline Yönetimi:**
  - Birden fazla komut pipe operatörü ile birbirine bağlanıyorsa, her bir komut arasında pipe (boru) oluştur.
  - Her pipe, bir komutun çıktısını diğer komutun girdisine bağlar.
  - Birden fazla çocuk süreç fork edilerek, pipe'lar üzerinden veri akışı sağlanır.

---

### 5. Hata Yönetimi ve Sinyal Entegrasyonu
- **Hata Kontrolleri:**
  - Fork, execve, dosya açma, getcwd gibi sistem çağrılarında hata kontrolleri yap.
  - Uygun hata mesajları basarak veya çıkış kodları döndürerek hata durumlarını yönet.

- **Sinyal Yönetimi ile İşbirliği:**
  - Execute süreci sırasında, sinyaller (örneğin, Ctrl+C) çocuk süreçlerde farklı şekilde ele alınmalı.
  - Foreground süreçlerde sinyal yönetimi ile etkileşim sağlanabilir.

---

### 6. Modüler Yapı ve Fonksiyon Dağılımı
- **Executor Modülü:**
  - Ana execute fonksiyonu, yukarıdaki adımları sırasıyla çağırır.
  - Bu fonksiyon, hem built-in hem de dış komutları çalıştıracak şekilde iki farklı alt fonksiyona ayrılabilir.

- **Redirection ve Pipe Fonksiyonları:**
  - Her yönlendirme operatörü için ayrı yardımcı fonksiyonlar yaz.
  - Pipe kurulumunu ve çocuk süreçler arası bağlantıyı yöneten fonksiyonlar oluştur.

- **Environment Dönüşümü:**
  - execve çağrısına geçmeden önce, environment linked list yapısını char ** dizisine dönüştüren fonksiyonun entegrasyonu (daha önce oluşturduğumuz env_list_to_array) sağlanır.

---

### 7. Test ve Entegrasyon
- **Unit Testler:**
  - Her bir alt modülü (örneğin, redirection, pipe, environment dönüşümü) ayrı ayrı test et.
- **Bütünleşik Testler:**
  - Parser tarafından oluşturulan komut yapılarını alarak, execute modülünün tüm adımlarının entegrasyonunu test et.

---
