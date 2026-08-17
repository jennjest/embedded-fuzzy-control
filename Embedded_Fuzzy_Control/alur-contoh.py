import matplotlib.pyplot as plt
import matplotlib.patches as patches

# 1. SETUP FIGURE (Ukuran Landscape & Background Putih)
fig = plt.figure(figsize=(14, 7))
fig.patch.set_facecolor('white')

# Judul Utama
fig.suptitle('ALUR PROSES FUZZY LOGIC: Temperatur 27°C', 
             fontsize=22, fontweight='bold', y=0.95)

# ==========================================
# 2. KOMPONEN KOTAK (INPUT, RULE, DEFUZZ, OUTPUT)
# ==========================================

# KOTAK INPUT (Kiri)
ax_in = fig.add_axes([0.02, 0.35, 0.12, 0.3])
ax_in.axis('off')
ax_in.text(0.5, 0.5, "INPUT SENSOR\n\nSuhu:\n27°C\n\nKelembapan:\nSedang", 
           ha='center', va='center', fontsize=14, 
           bbox=dict(boxstyle="round,pad=1", fc="white", ec="black", lw=1.5))

# KOTAK RULE 1 (Tengah Atas)
ax_r1 = fig.add_axes([0.50, 0.60, 0.16, 0.22])
ax_r1.axis('off')
ax_r1.text(0.5, 0.5, "RULE 1\nIF (Suhu Sedang) AND (Kelembapan Sedang)\nTHEN (Hijau)\n\nMIN(0.6, 1.0) = 0.6", 
           ha='center', va='center', fontsize=10, 
           bbox=dict(boxstyle="round,pad=0.8", fc="#d9ead3", ec="black", lw=1.5))

# KOTAK RULE 2 (Tengah Bawah)
ax_r2 = fig.add_axes([0.50, 0.20, 0.16, 0.22])
ax_r2.axis('off')
ax_r2.text(0.5, 0.5, "RULE 2\nIF (Suhu Panas) AND (Kelembapan Sedang)\nTHEN (Kuning)\n\nMIN(0.4, 1.0) = 0.4", 
           ha='center', va='center', fontsize=10, 
           bbox=dict(boxstyle="round,pad=0.8", fc="#fff2cc", ec="black", lw=1.5))

# KOTAK DEFUZZIFIKASI (Kanan)
ax_d = fig.add_axes([0.73, 0.37, 0.12, 0.26])
ax_d.axis('off')
defuzz_text = (
    "DEFUZZIFIKASI\n(Rata-rata Berbobot)\n\n"
    "z = [(0.6 × 100) + (0.4 × 50)]\n/ (0.6 + 0.4)\n\n"
    "z = 80"
)
ax_d.text(0.5, 0.5, defuzz_text, 
          ha='center', va='center', fontsize=9, 
          bbox=dict(boxstyle="round,pad=0.7", fc="white", ec="black", lw=1.5))

# KOTAK OUTPUT AKHIR (Paling Kanan)
ax_o = fig.add_axes([0.88, 0.38, 0.09, 0.24])
ax_o.axis('off')
ax_o.text(0.5, 0.5, "OUTPUT\n\nNilai Crisp:\n80\n\nHIJAU", 
          ha='center', va='center', fontsize=11, fontweight='bold',
          bbox=dict(boxstyle="round,pad=0.7", fc="#f3f3f3", ec="black", lw=1.5))

# ==========================================
# 3. GRAFIK MEMBERSHIP FUNCTION (Presisi Matematis)
# ==========================================

# GRAFIK FUZZIFIKASI SUHU
ax_f1 = fig.add_axes([0.22, 0.55, 0.2, 0.3])
ax_f1.set_title("Fuzzifikasi Suhu", fontsize=14, fontweight='bold')
# Tambahan: Segitiga Dingin
ax_f1.plot([5, 15, 25], [0, 1, 0], label='Dingin', color='#2ca02c', lw=2)
# Membangun garis segitiga agar saat x=27, y benar-benar 0.6 dan 0.4
ax_f1.plot([15, 24, 31.5], [0, 1, 0], label='Sedang', color='#1f77b4', lw=2)
ax_f1.plot([23, 33, 45], [0, 1, 1], label='Panas', color='#ff7f0e', lw=2)

# Garis vertikal presisi di angka 27
ax_f1.axvline(27, color='black', linestyle='--', lw=1.5)
# Titik potong presisi
ax_f1.plot(27, 0.6, 'ko', markersize=8) 
ax_f1.plot(27, 0.4, 'ko', markersize=8) 
# Teks penjelasan titik
ax_f1.text(28, 0.6, "μ_Sedang(27) = 0.6", va='center', fontsize=11)
ax_f1.text(28, 0.4, "μ_Panas(27) = 0.4", va='center', fontsize=11)

ax_f1.set_ylim(0, 1.1)
ax_f1.set_xlim(5, 45) # Diperlebar agar bagian 'Dingin' terlihat
ax_f1.set_ylabel("Derajat Keanggotaan")
ax_f1.legend(loc='upper left', fontsize=9)

# GRAFIK FUZZIFIKASI KELEMBAPAN
ax_f2 = fig.add_axes([0.22, 0.15, 0.2, 0.3])
ax_f2.set_title("Fuzzifikasi Kelembapan", fontsize=14, fontweight='bold')
# Tambahan: 3 Kategori Kelembapan (Kering, Sedang, Basah)
ax_f2.plot([0, 20, 50], [0, 1, 0], label='Kering', color='#d62728', lw=2)
ax_f2.plot([20, 50, 80], [0, 1, 0], label='Sedang', color='#1f77b4', lw=2)
ax_f2.plot([50, 80, 100], [0, 1, 0], label='Basah', color='#2ca02c', lw=2)

ax_f2.axvline(50, color='black', linestyle='--', lw=1.5)
ax_f2.plot(50, 1.0, 'ko', markersize=8)
ax_f2.text(52, 1.0, "μ_Sedang(Input) = 1.0", va='center', fontsize=11)

ax_f2.set_ylim(0, 1.1)
ax_f2.set_xlim(0, 100) # Diperlebar skala persentase 0-100
ax_f2.set_ylabel("Derajat Keanggotaan")
ax_f2.legend(loc='upper left', fontsize=9)

# ==========================================
# 4. PANAH PENGHUBUNG ALUR
# ==========================================
def draw_arrow(fig, posA, posB):
    arrow = patches.FancyArrowPatch(posA, posB, transform=fig.transFigure, 
                                    mutation_scale=12, color='#555555', 
                                    arrowstyle='-|>', lw=1.5)
    fig.patches.append(arrow)

# Panah Input -> Fuzzifikasi
draw_arrow(fig, (0.14, 0.5), (0.21, 0.7))  # Ke Suhu
draw_arrow(fig, (0.14, 0.5), (0.21, 0.3))  # Ke Kelembapan

# Panah Fuzzifikasi -> Inferensi (Rule)
draw_arrow(fig, (0.43, 0.7), (0.50, 0.68))  # Suhu ke Rule 1
draw_arrow(fig, (0.43, 0.3), (0.50, 0.28))  # Kelembapan ke Rule 2

# Panah Inferensi -> Defuzzifikasi
draw_arrow(fig, (0.68, 0.71), (0.73, 0.58)) # Rule 1 ke Defuzz
draw_arrow(fig, (0.68, 0.31), (0.73, 0.46)) # Rule 2 ke Defuzz

# Panah Defuzzifikasi -> Output
draw_arrow(fig, (0.85, 0.5), (0.88, 0.5))

# Tampilkan Hasil
plt.show()