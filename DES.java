

import javax.crypto.Cipher;
import javax.crypto.spec.SecretKeySpec;
import javax.swing.*;
import javax.swing.border.Border;
import javax.swing.border.EmptyBorder;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.File;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.Base64;
import java.util.Random;

public class DESApplication extends JFrame {

    // --- Biến giao diện ---
    private JTextArea txtEncInput, txtEncResult;
    private JTextField txtEncKey;
    private JComboBox<String> cbEncFormat;
    private JComboBox<String> cbEncKeyFormat;

    private JTextArea txtDecInput, txtDecResult;
    private JTextField txtDecKey;
    private JComboBox<String> cbDecFormat;
    private JComboBox<String> cbDecKeyFormat;

    // --- Bảng màu Modern Flat UI ---
    private final Color PRIMARY_COLOR = Color.decode("#4A90E2");   
    private final Color SUCCESS_COLOR = Color.decode("#2ECC71");   
    private final Color DANGER_COLOR = Color.decode("#E74C3C");    
    private final Color SECONDARY_COLOR = Color.decode("#95A5A6"); 
    
    private final Color ENC_BG_COLOR = Color.decode("#EAF2F8");    
    private final Color DEC_BG_COLOR = Color.decode("#FDEDEC");    

    public DESApplication() {
        setTitle("Chương trình Mã hóa & Giải mã DES - Đồ án cuối kì");
        setSize(1000, 750); 
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);
        getContentPane().setBackground(Color.WHITE);

        Font mainFont = new Font("Segoe UI", Font.PLAIN, 15);
        UIManager.put("Label.font", mainFont);
        UIManager.put("ComboBox.font", mainFont);
        UIManager.put("TabbedPane.font", new Font("Segoe UI", Font.BOLD, 16));

        JTabbedPane tabbedPane = new JTabbedPane();
        tabbedPane.setBackground(Color.WHITE);
        tabbedPane.setFocusable(false);

        tabbedPane.addTab("  Mã Hóa DES  ", createEncryptPanel());
        tabbedPane.addTab("  Giải Mã DES  ", createDecryptPanel());

        add(tabbedPane);
    }

    // ==========================================
    // 1. GIAO DIỆN MÃ HÓA
    // ==========================================
    private JPanel createEncryptPanel() {
        JPanel panel = new JPanel(new BorderLayout(15, 15));
        panel.setBorder(new EmptyBorder(20, 30, 20, 30));
        panel.setBackground(ENC_BG_COLOR); 

        JLabel lblTitle = new JLabel("CÔNG CỤ MÃ HÓA DỮ LIỆU", SwingConstants.CENTER);
        lblTitle.setFont(new Font("Segoe UI", Font.BOLD, 26));
        lblTitle.setForeground(PRIMARY_COLOR);
        panel.add(lblTitle, BorderLayout.NORTH);

        JPanel contentPanel = new JPanel();
        contentPanel.setLayout(new BoxLayout(contentPanel, BoxLayout.Y_AXIS));
        contentPanel.setBackground(ENC_BG_COLOR);

        // -- VĂN BẢN GỐC -- 
        JPanel pnlInputTop = new JPanel(new FlowLayout(FlowLayout.LEFT, 0, 5));
        pnlInputTop.setBackground(ENC_BG_COLOR);
        pnlInputTop.add(createLabel("Văn bản gốc cần mã hóa:"));
        pnlInputTop.add(Box.createHorizontalStrut(15));
        pnlInputTop.add(new RoundedButton("Tải tệp (.txt)", PRIMARY_COLOR, e -> loadFileToTextArea(txtEncInput)));
        pnlInputTop.add(Box.createHorizontalStrut(10));
        pnlInputTop.add(new RoundedButton("Lưu văn bản", SECONDARY_COLOR, e -> saveTextToFile(txtEncInput.getText(), "Lưu văn bản gốc")));
        contentPanel.add(pnlInputTop);
        
        txtEncInput = createModernTextArea();
        contentPanel.add(new JScrollPane(txtEncInput));
        contentPanel.add(Box.createVerticalStrut(20));

        // -- KHÓA -- 
        JPanel pnlKey = new JPanel(new FlowLayout(FlowLayout.LEFT, 0, 5));
        pnlKey.setBackground(ENC_BG_COLOR);
        pnlKey.add(createLabel("Khóa bí mật: "));
        
        cbEncKeyFormat = new JComboBox<>(new String[]{"Text (8 ký tự)", "Hex (16 ký tự)"});
        cbEncKeyFormat.setPreferredSize(new Dimension(140, 30));
        pnlKey.add(cbEncKeyFormat);
        pnlKey.add(Box.createHorizontalStrut(10));
        
        txtEncKey = createModernTextField();
        pnlKey.add(txtEncKey);
        pnlKey.add(Box.createHorizontalStrut(10));
        
        pnlKey.add(new RoundedButton("Sinh khóa", PRIMARY_COLOR, e -> {
            String format = (String) cbEncKeyFormat.getSelectedItem();
            txtEncKey.setText(generateRandomKey(format));
        }));
        pnlKey.add(Box.createHorizontalStrut(10));
        
        pnlKey.add(new RoundedButton("Lưu khóa", SECONDARY_COLOR, e -> {
            String format = (String) cbEncKeyFormat.getSelectedItem();
            if (validateKey(txtEncKey.getText(), format)) {
                saveTextToFile(txtEncKey.getText(), "Lưu khóa DES");
            }
        }));
        
        pnlKey.add(Box.createHorizontalStrut(10));
        pnlKey.add(new RoundedButton("Tải khóa", SECONDARY_COLOR, e -> loadFileToTextField(txtEncKey)));
        contentPanel.add(pnlKey);
        contentPanel.add(Box.createVerticalStrut(20));

        // -- ĐỊNH DẠNG & THỰC THI --
        JPanel pnlAction = new JPanel(new FlowLayout(FlowLayout.LEFT, 0, 10));
        pnlAction.setBackground(ENC_BG_COLOR);
        pnlAction.add(createLabel("Định dạng đầu ra:"));
        pnlAction.add(Box.createHorizontalStrut(15));
        
        cbEncFormat = new JComboBox<>(new String[]{"Base64", "Hex"});
        cbEncFormat.setPreferredSize(new Dimension(120, 35)); 
        pnlAction.add(cbEncFormat);
        pnlAction.add(Box.createHorizontalStrut(30));
        
        RoundedButton btnEncrypt = new RoundedButton(" MÃ HÓA DỮ LIỆU ", SUCCESS_COLOR, null);
        btnEncrypt.setFont(new Font("Segoe UI", Font.BOLD, 16));
        pnlAction.add(btnEncrypt);
        pnlAction.add(Box.createHorizontalStrut(15));
        
        pnlAction.add(new RoundedButton("Xóa trắng", SECONDARY_COLOR, e -> { txtEncInput.setText(""); txtEncKey.setText(""); txtEncResult.setText(""); }));
        contentPanel.add(pnlAction);
        contentPanel.add(Box.createVerticalStrut(20));

        // -- KẾT QUẢ -- 
        JPanel pnlResultTop = new JPanel(new FlowLayout(FlowLayout.LEFT, 0, 5));
        pnlResultTop.setBackground(ENC_BG_COLOR);
        pnlResultTop.add(createLabel("Kết quả mã hóa:"));
        pnlResultTop.add(Box.createHorizontalStrut(15));
        pnlResultTop.add(new RoundedButton("Lưu kết quả", PRIMARY_COLOR, e -> saveTextToFile(txtEncResult.getText(), "Lưu kết quả mã hóa")));
        contentPanel.add(pnlResultTop);
        
        txtEncResult = createModernTextArea();
        txtEncResult.setEditable(false);
        txtEncResult.setBackground(Color.decode("#FFFFFF"));
        contentPanel.add(new JScrollPane(txtEncResult));

        panel.add(contentPanel, BorderLayout.CENTER);

        // Sự kiện Mã hóa
        btnEncrypt.addActionListener(e -> {
            if (txtEncInput.getText().trim().isEmpty()) {
                JOptionPane.showMessageDialog(this, "Vui lòng nhập văn bản cần mã hóa!", "Cảnh báo", JOptionPane.WARNING_MESSAGE);
                return;
            }
            String keyFormat = (String) cbEncKeyFormat.getSelectedItem();
            if (validateKey(txtEncKey.getText(), keyFormat)) {
                try {
                    String outputFormat = (String) cbEncFormat.getSelectedItem();
                    String cipherText = encryptDES(txtEncInput.getText(), txtEncKey.getText(), keyFormat, outputFormat);
                    txtEncResult.setText(cipherText);
                    
                    // [THÊM MỚI] Thông báo mã hóa thành công
                    JOptionPane.showMessageDialog(this, "Mã hóa thành công.", "Thông báo", JOptionPane.INFORMATION_MESSAGE);
                    
                } catch (Exception ex) {
                    JOptionPane.showMessageDialog(this, "Lỗi mã hóa: " + ex.getMessage(), "Lỗi", JOptionPane.ERROR_MESSAGE);
                }
            }
        });

        return panel;
    }

    // ==========================================
    // 2. GIAO DIỆN GIẢI MÃ
    // ==========================================
    private JPanel createDecryptPanel() {
        JPanel panel = new JPanel(new BorderLayout(15, 15));
        panel.setBorder(new EmptyBorder(20, 30, 20, 30));
        panel.setBackground(DEC_BG_COLOR); 

        JLabel lblTitle = new JLabel("CÔNG CỤ GIẢI MÃ DỮ LIỆU", SwingConstants.CENTER);
        lblTitle.setFont(new Font("Segoe UI", Font.BOLD, 26));
        lblTitle.setForeground(DANGER_COLOR);
        panel.add(lblTitle, BorderLayout.NORTH);

        JPanel contentPanel = new JPanel();
        contentPanel.setLayout(new BoxLayout(contentPanel, BoxLayout.Y_AXIS));
        contentPanel.setBackground(DEC_BG_COLOR);

        // -- VĂN BẢN MÃ HÓA --
        JPanel pnlInputTop = new JPanel(new FlowLayout(FlowLayout.LEFT, 0, 5));
        pnlInputTop.setBackground(DEC_BG_COLOR);
        pnlInputTop.add(createLabel("Văn bản đã mã hóa:"));
        pnlInputTop.add(Box.createHorizontalStrut(15));
        pnlInputTop.add(new RoundedButton("Tải tệp (.txt)", PRIMARY_COLOR, e -> loadFileToTextArea(txtDecInput)));
        contentPanel.add(pnlInputTop);
        
        txtDecInput = createModernTextArea();
        contentPanel.add(new JScrollPane(txtDecInput));
        contentPanel.add(Box.createVerticalStrut(20));

        // -- KHÓA --
        JPanel pnlKey = new JPanel(new FlowLayout(FlowLayout.LEFT, 0, 5));
        pnlKey.setBackground(DEC_BG_COLOR);
        pnlKey.add(createLabel("Khóa giải mã: "));
        
        cbDecKeyFormat = new JComboBox<>(new String[]{"Text (8 ký tự)", "Hex (16 ký tự)"});
        cbDecKeyFormat.setPreferredSize(new Dimension(140, 30));
        pnlKey.add(cbDecKeyFormat);
        pnlKey.add(Box.createHorizontalStrut(10));
        
        txtDecKey = createModernTextField();
        pnlKey.add(txtDecKey);
        pnlKey.add(Box.createHorizontalStrut(15));
        pnlKey.add(new RoundedButton("Tải khóa", SECONDARY_COLOR, e -> loadFileToTextField(txtDecKey)));
        contentPanel.add(pnlKey);
        contentPanel.add(Box.createVerticalStrut(20));

        // -- ĐỊNH DẠNG & THỰC THI --
        JPanel pnlAction = new JPanel(new FlowLayout(FlowLayout.LEFT, 0, 10));
        pnlAction.setBackground(DEC_BG_COLOR);
        pnlAction.add(createLabel("Định dạng đầu vào:"));
        pnlAction.add(Box.createHorizontalStrut(15));
        
        cbDecFormat = new JComboBox<>(new String[]{"Base64", "Hex"});
        cbDecFormat.setPreferredSize(new Dimension(120, 35));
        pnlAction.add(cbDecFormat);
        pnlAction.add(Box.createHorizontalStrut(30));
        
        RoundedButton btnDecrypt = new RoundedButton(" GIẢI MÃ DỮ LIỆU ", DANGER_COLOR, null);
        btnDecrypt.setFont(new Font("Segoe UI", Font.BOLD, 16));
        pnlAction.add(btnDecrypt);
        pnlAction.add(Box.createHorizontalStrut(15));
        
        pnlAction.add(new RoundedButton("Xóa trắng", SECONDARY_COLOR, e -> { txtDecInput.setText(""); txtDecKey.setText(""); txtDecResult.setText(""); }));
        contentPanel.add(pnlAction);
        contentPanel.add(Box.createVerticalStrut(20));

        // -- KẾT QUẢ --
        JPanel pnlResultTop = new JPanel(new FlowLayout(FlowLayout.LEFT, 0, 5));
        pnlResultTop.setBackground(DEC_BG_COLOR);
        pnlResultTop.add(createLabel("Kết quả giải mã (Văn bản gốc):"));
        pnlResultTop.add(Box.createHorizontalStrut(15));
        pnlResultTop.add(new RoundedButton("Lưu kết quả", PRIMARY_COLOR, e -> saveTextToFile(txtDecResult.getText(), "Lưu kết quả giải mã")));
        contentPanel.add(pnlResultTop);
        
        txtDecResult = createModernTextArea();
        txtDecResult.setEditable(false);
        txtDecResult.setBackground(Color.decode("#FFFFFF"));
        contentPanel.add(new JScrollPane(txtDecResult));

        panel.add(contentPanel, BorderLayout.CENTER);

        // Sự kiện Giải mã
        btnDecrypt.addActionListener(e -> {
            String cipherText = txtDecInput.getText().trim();
            String keyStr = txtDecKey.getText();

            if (cipherText.isEmpty()) {
                JOptionPane.showMessageDialog(this, "Vui lòng nhập văn bản cần giải mã!", "Cảnh báo", JOptionPane.WARNING_MESSAGE);
                return;
            }

            String keyFormat = (String) cbDecKeyFormat.getSelectedItem();
            if (validateKey(keyStr, keyFormat)) {
                try {
                    String inputFormat = (String) cbDecFormat.getSelectedItem();
                    byte[] decodedBytes;
                    
                    try {
                        if ("Base64".equals(inputFormat)) {
                            decodedBytes = Base64.getDecoder().decode(cipherText);
                        } else {
                            decodedBytes = hexToBytes(cipherText);
                        }
                    } catch (IllegalArgumentException ex) {
                        throw new Exception("VĂN BẢN SAI: Dữ liệu mã hóa không hợp lệ (Không đúng định dạng " + inputFormat + ").");
                    }

                    try {
                        byte[] keyBytes;
                        if ("Text (8 ký tự)".equals(keyFormat)) {
                            keyBytes = keyStr.getBytes(StandardCharsets.UTF_8);
                        } else {
                            keyBytes = hexToBytes(keyStr);
                        }
                        
                        SecretKeySpec key = new SecretKeySpec(keyBytes, "DES");
                        Cipher cipher = Cipher.getInstance("DES/ECB/PKCS5Padding");
                        cipher.init(Cipher.DECRYPT_MODE, key);
                        
                        byte[] decryptedBytes = cipher.doFinal(decodedBytes);
                        txtDecResult.setText(new String(decryptedBytes, StandardCharsets.UTF_8));
                        
                        // [THÊM MỚI] Thông báo giải mã thành công
                        JOptionPane.showMessageDialog(this, "Giải mã thành công.", "Thông báo", JOptionPane.INFORMATION_MESSAGE);
                        
                    } catch (javax.crypto.BadPaddingException | javax.crypto.IllegalBlockSizeException ex) {
                        throw new Exception("KHÓA SAI: Khóa bạn nhập không khớp với khóa đã dùng để mã hóa!");
                    }

                } catch (Exception ex) {
                    JOptionPane.showMessageDialog(this, ex.getMessage(), "Lỗi Giải Mã", JOptionPane.ERROR_MESSAGE);
                }
            }
        });

        return panel;
    }

    // ==========================================
    // 3. UI COMPONENTS & CUSTOM CLASSES
    // ==========================================

    private JLabel createLabel(String text) {
        JLabel label = new JLabel(text);
        label.setFont(new Font("Segoe UI", Font.BOLD, 14));
        label.setForeground(Color.decode("#34495E")); 
        return label;
    }

    private JTextArea createModernTextArea() {
        JTextArea ta = new JTextArea(5, 20);
        ta.setLineWrap(true);
        ta.setFont(new Font("Consolas", Font.PLAIN, 16));
        Border line = BorderFactory.createLineBorder(Color.decode("#CED4DA"), 1);
        Border empty = new EmptyBorder(10, 10, 10, 10);
        ta.setBorder(BorderFactory.createCompoundBorder(line, empty));
        return ta;
    }

    private JTextField createModernTextField() {
        JTextField tf = new JTextField(20); 
        tf.setFont(new Font("Consolas", Font.BOLD, 16));
        tf.setPreferredSize(new Dimension(260, 35));
        Border line = BorderFactory.createLineBorder(Color.decode("#CED4DA"), 1);
        Border empty = new EmptyBorder(5, 10, 5, 10);
        tf.setBorder(BorderFactory.createCompoundBorder(line, empty));
        return tf;
    }

    class RoundedButton extends JButton {
        private Color normalColor;
        private Color hoverColor;

        public RoundedButton(String text, Color bgColor, java.awt.event.ActionListener action) {
            super(text);
            this.normalColor = bgColor;
            this.hoverColor = bgColor.darker(); 
            
            setForeground(Color.WHITE);
            setFont(new Font("Segoe UI", Font.BOLD, 13));
            setFocusPainted(false);
            setContentAreaFilled(false);
            setBorderPainted(false);
            setOpaque(false);
            setCursor(new Cursor(Cursor.HAND_CURSOR));
            
            setBorder(new EmptyBorder(8, 15, 8, 15));

            if (action != null) {
                addActionListener(action);
            }

            addMouseListener(new MouseAdapter() {
                @Override
                public void mouseEntered(MouseEvent e) {
                    normalColor = hoverColor;
                    repaint();
                }
                @Override
                public void mouseExited(MouseEvent e) {
                    normalColor = bgColor;
                    repaint();
                }
            });
        }

        @Override
        protected void paintComponent(Graphics g) {
            Graphics2D g2 = (Graphics2D) g.create();
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            g2.setColor(normalColor);
            g2.fillRoundRect(0, 0, getWidth(), getHeight(), 15, 15);
            g2.dispose();
            super.paintComponent(g);
        }
    }

    // ==========================================
    // 4. CÁC HÀM XỬ LÝ LOGIC VÀ THUẬT TOÁN
    // ==========================================

    private boolean validateKey(String key, String format) {
        if (key == null || key.trim().isEmpty()) {
            JOptionPane.showMessageDialog(this, "Vui lòng nhập khóa!", "Thiếu khóa", JOptionPane.WARNING_MESSAGE);
            return false;
        }
        
        if ("Text (8 ký tự)".equals(format)) {
            int len = key.length();
            if (len < 8) {
                JOptionPane.showMessageDialog(this, "Khóa Text đang thiếu " + (8 - len) + " ký tự.\nYêu cầu đúng 8 ký tự Text.", "Lỗi Khóa", JOptionPane.ERROR_MESSAGE);
                return false;
            } else if (len > 8) {
                JOptionPane.showMessageDialog(this, "Khóa Text đang thừa " + (len - 8) + " ký tự.\nYêu cầu đúng 8 ký tự Text.", "Lỗi Khóa", JOptionPane.ERROR_MESSAGE);
                return false;
            }
        } else {
            key = key.replaceAll("\\s+", ""); 
            int len = key.length();
            if (len < 16) {
                JOptionPane.showMessageDialog(this, "Khóa Hex đang thiếu " + (16 - len) + " ký tự.\nYêu cầu đúng 16 ký tự Hex.", "Lỗi Khóa", JOptionPane.ERROR_MESSAGE);
                return false;
            } else if (len > 16) {
                JOptionPane.showMessageDialog(this, "Khóa Hex đang thừa " + (len - 16) + " ký tự.\nYêu cầu đúng 16 ký tự Hex.", "Lỗi Khóa", JOptionPane.ERROR_MESSAGE);
                return false;
            }
            if (!key.matches("^[0-9a-fA-F]+$")) {
                JOptionPane.showMessageDialog(this, "Khóa không hợp lệ!\nVui lòng chỉ nhập các ký tự Hex (0-9, A-F, a-f).", "Lỗi Khóa", JOptionPane.ERROR_MESSAGE);
                return false;
            }
        }
        return true;
    }

    private String generateRandomKey(String format) {
        Random rnd = new Random();
        if ("Text (8 ký tự)".equals(format)) {
            String chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*";
            StringBuilder sb = new StringBuilder(8);
            for(int i = 0; i < 8; i++) sb.append(chars.charAt(rnd.nextInt(chars.length())));
            return sb.toString();
        } else {
            String chars = "0123456789ABCDEF";
            StringBuilder sb = new StringBuilder(16);
            for(int i = 0; i < 16; i++) sb.append(chars.charAt(rnd.nextInt(chars.length())));
            return sb.toString();
        }
    }

    private String encryptDES(String plainText, String keyStr, String keyFormat, String outputFormat) throws Exception {
        byte[] keyBytes;
        if ("Text (8 ký tự)".equals(keyFormat)) {
            keyBytes = keyStr.getBytes(StandardCharsets.UTF_8);
        } else {
            keyBytes = hexToBytes(keyStr);
        }
        
        SecretKeySpec key = new SecretKeySpec(keyBytes, "DES");
        Cipher cipher = Cipher.getInstance("DES/ECB/PKCS5Padding");
        cipher.init(Cipher.ENCRYPT_MODE, key);
        byte[] encryptedBytes = cipher.doFinal(plainText.getBytes(StandardCharsets.UTF_8));
        
        if ("Base64".equals(outputFormat)) {
            return Base64.getEncoder().encodeToString(encryptedBytes);
        } else {
            return bytesToHex(encryptedBytes);
        }
    }

    private String bytesToHex(byte[] bytes) {
        StringBuilder hexString = new StringBuilder();
        for (byte b : bytes) {
            String hex = Integer.toHexString(0xFF & b);
            if (hex.length() == 1) {
                hexString.append('0');
            }
            hexString.append(hex.toUpperCase());
        }
        return hexString.toString();
    }

    private byte[] hexToBytes(String s) throws IllegalArgumentException {
        s = s.replaceAll("\\s+", ""); 
        int len = s.length();
        if (len % 2 != 0) throw new IllegalArgumentException("Độ dài chuỗi Hex không hợp lệ");
        byte[] data = new byte[len / 2];
        for (int i = 0; i < len; i += 2) {
            int high = Character.digit(s.charAt(i), 16);
            int low = Character.digit(s.charAt(i + 1), 16);
            if (high == -1 || low == -1) throw new IllegalArgumentException("Chứa ký tự không phải Hex");
            data[i / 2] = (byte) ((high << 4) + low);
        }
        return data;
    }

    private void saveTextToFile(String content, String title) {
        if (content.isEmpty()) {
            JOptionPane.showMessageDialog(this, "Không có dữ liệu để lưu!", "Thông báo", JOptionPane.WARNING_MESSAGE);
            return;
        }
        JFileChooser fileChooser = new JFileChooser();
        fileChooser.setDialogTitle(title);
        fileChooser.setFileFilter(new FileNameExtensionFilter("Văn bản (*.txt)", "txt"));
        
        if (fileChooser.showSaveDialog(this) == JFileChooser.APPROVE_OPTION) {
            try {
                File file = fileChooser.getSelectedFile();
                if (!file.getName().endsWith(".txt")) {
                    file = new File(file.getAbsolutePath() + ".txt");
                }
                Files.write(file.toPath(), content.getBytes(StandardCharsets.UTF_8));
                JOptionPane.showMessageDialog(this, "Lưu thành công: " + file.getAbsolutePath(), "Thành công", JOptionPane.INFORMATION_MESSAGE);
            } catch (Exception ex) {
                JOptionPane.showMessageDialog(this, "Lỗi khi lưu file: " + ex.getMessage(), "Lỗi", JOptionPane.ERROR_MESSAGE);
            }
        }
    }

    private void loadFileToTextField(JTextField textField) {
        JFileChooser fileChooser = new JFileChooser();
        fileChooser.setFileFilter(new FileNameExtensionFilter("Văn bản (*.txt)", "txt"));
        
        if (fileChooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
            try {
                String content = Files.readString(fileChooser.getSelectedFile().toPath(), StandardCharsets.UTF_8);
                textField.setText(content.trim());
            } catch (Exception ex) {
                JOptionPane.showMessageDialog(this, "Không thể đọc file.", "Lỗi", JOptionPane.ERROR_MESSAGE);
            }
        }
    }

    private void loadFileToTextArea(JTextArea textArea) {
        JFileChooser fileChooser = new JFileChooser();
        fileChooser.setFileFilter(new FileNameExtensionFilter("Văn bản (*.txt)", "txt"));

        if (fileChooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
            File file = fileChooser.getSelectedFile();
            try {
                if (file.getName().endsWith(".txt")) {
                    String content = Files.readString(file.toPath(), StandardCharsets.UTF_8);
                    textArea.setText(content);
                } else {
                    JOptionPane.showMessageDialog(this, "Vui lòng chọn file .txt", "Lỗi", JOptionPane.ERROR_MESSAGE);
                }
            } catch (Exception ex) {
                JOptionPane.showMessageDialog(this, "Không thể đọc file!\nChi tiết: " + ex.getMessage(), "Lỗi", JOptionPane.ERROR_MESSAGE);
            }
        }
    }

    public static void main(String[] args) {
        try {
            UIManager.setLookAndFeel(UIManager.getCrossPlatformLookAndFeelClassName());
        } catch (Exception e) { e.printStackTrace(); }

        SwingUtilities.invokeLater(() -> {
            new DESApplication().setVisible(true);
        });
    }
}
