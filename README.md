# Minishell

İşte adım adım olanlar:


   1. Sinyal Gönderimi: Kullanıcı Ctrl+C'ye bastığında, SIGINT sinyali hem heredoc girdisini bekleyen alt sürece hem de bu alt
      süreci bekleyen ana shell sürecine gönderilir.
   2. Alt Sürecin Tepkisi (Doğru Çalışıyor):
       * Alt süreç, sinyali heredoc_child_sigint_handler ile yakalar.
       * Kendi g_signal_flag değişkenini ayarlar.
       * Okuma döngüsünden çıkar.
       * Sinyal bayrağını kontrol eder ve exit(130) ile kendini sonlandırır. Bu, "sinyal ile kesildi" anlamına gelen standart
         bir çıkış kodudur.
   3. Ana Shell Sürecinin Tepkisi (Sorun Burada):
       * Ana shell süreci, o anda waitpid() sistem çağrısı ile alt sürecin bitmesini beklemektedir.
       * Ana süreç, sinyali interaktif mod için ayarlanmış olan sigint_handler (signal_handlers.c içinde) ile yakalar.
       * Bu sigint_handler, ana sürecin kendi g_signal_flag değişkenini SIGINT olarak ayarlar ve ekrana yeni bir komut istemi
         basmak için readline kütüphanesinin fonksiyonlarını (rl_on_new_line, rl_redisplay vb.) çağırır.
   4. Yarış Durumu (Race Condition) ve Hatalı Çıkış:
       * Alt süreç sonlandığında, ana süreç waitpid()'den döner ve heredoc işleminin başarısız olduğunu anlar. Komutun geri
         kalanını iptal eder ve ana döngüye (input_loop) geri döner.
       * Ana döngü yeni bir komut okumak için readline() fonksiyonunu çağırdığında, readline kütüphanesi, daha önce
         sigint_handler tarafından çağrılan rl_* fonksiyonları nedeniyle kafası karışmış bir duruma girer.
       * En önemlisi, ana sürecin g_signal_flag'ı hala SIGINT olarak ayarlıdır ve hiçbir zaman sıfırlanmamıştır.
       * Bu durum, readline'ın NULL döndürmesine neden olabilir, bu da program tarafından "kullanıcı Ctrl+D'ye bastı (EOF)"
         olarak yorumlanır.
       * main.c'deki input_loop fonksiyonu readline'dan NULL aldığında, break; komutu ile döngüyü kırar ve shell programı
         tamamen sonlanır.

  Özetle:


  Sorun, Ctrl+C sinyalinin hem ana süreç hem de alt süreç tarafından aynı anda ve farklı şekillerde ele alınmasıdır. Ana
  sürecin sinyal işleyicisi, readline kütüphanesinin durumunu bozuyor ve g_signal_flag'ı temizlemeden bırakıyor. Bu da bir
  sonraki döngüde shell'in yanlışlıkla EOF (dosya sonu) algılayıp kapanmasına neden oluyor.
