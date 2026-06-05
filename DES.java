import javax.crypto.Cipher;
import javax.crypto.spec.SecretKeySpec;
import javax.swing.*;
import javax.swing.border.TitledBorder;
import java.awt.*;
import java.io.File;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.Base64;

public class DESStringTool extends JFrame {

    // --- Khai báo các thành phần giao diện ---
    // Phần Mã hóa
    private JTextArea txtOriginal, txtEncryptedResult;
    private JTextField txtEncryptKey;
    private JComboBox<String> cbEncryptFormat;
    private JButton btnEncrypt, btnClearEnc, btnSaveResultEnc;
    private JButton btnLoadTextEnc, btnLoadKeyEnc, btnSaveKeyEnc, btnGenKeyEnc;

    // Phần Giải mã
    private JTextArea txtEncryptedInput, txtDecryptedResult;
    private JTextField txtDecryptKey;
    private JComboBox<String> cbDecryptFormat;
    private JButton btnDecrypt, btnClearDec, btnSaveResultDec;
    private JButton btnLoadTextDec, btnLoadKeyDec, btnSaveKeyDec;

    public DESStringTool() {
        setTitle("DES String Tool");
        setSize(1000, 550); // Thu gọn chiều cao một chút vì đã bỏ Độ dài khóa
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);
        setLayout(new GridLayout(1, 2, 10, 10));
        getContentPane().setBackground(new Color(230, 240, 255));

        add(createEncryptionPanel());
        add(createDecryptionPanel());

        addEventHandlers();
    }

    // ================== PANEL MÃ HÓA ==================
    private JPanel createEncryptionPanel() {
        JPanel panel = new JPanel(new GridBagLayout());
        panel.setBorder(BorderFactory.createTitledBorder(null, "Mã hóa", TitledBorder.CENTER, TitledBorder.TOP,
                new Font("Arial", Font.BOLD, 16), new Color(100, 150, 255)));
        panel.setBackground(new Color(190, 225, 255));

        GridBagConstraints gbc = new GridBagConstraints();
        gbc.insets = new Insets(8, 8, 8, 8);
        gbc.fill = GridBagConstraints.HORIZONTAL;

        // Dòng 1: Văn bản gốc
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.weightx = 0.1;
        panel.add(new JLabel("Văn bản gốc:"), gbc);
        txtOriginal = new JTextArea(4, 25);
        txtOriginal.setLineWrap(true);
        gbc.gridx = 1;
        gbc.weightx = 0.9;
        panel.add(new JScrollPane(txtOriginal), gbc);

        // Nút tải tệp văn bản
        JPanel pnlLoadText = new JPanel(new FlowLayout(FlowLayout.RIGHT, 0, 0));
        pnlLoadText.setOpaque(false);
        btnLoadTextEnc = createStyledButton("Tải tệp văn bản", new Color(200, 230, 255), Color.BLACK);
        pnlLoadText.add(btnLoadTextEnc);
        gbc.gridx = 1;
        gbc.gridy = 1;
        panel.add(pnlLoadText, gbc);

        // Dòng 2: Nhập khóa (Đã bỏ Độ dài khóa)
        gbc.gridx = 0;
        gbc.gridy = 2;
        panel.add(new JLabel("Nhập khóa:"), gbc);
        txtEncryptKey = new JTextField("22F1E653B02CFFB4");
        gbc.gridx = 1;
        panel.add(txtEncryptKey, gbc);

        // Dòng 3: Nhóm nút cho Khóa
        JPanel pnlKeyBtns = new JPanel(new FlowLayout(FlowLayout.CENTER, 10, 0));
        pnlKeyBtns.setOpaque(false);
        btnLoadKeyEnc = createStyledButton("Tải khóa", new Color(240, 240, 240), Color.BLACK);
        btnSaveKeyEnc = createStyledButton("Lưu khóa", new Color(240, 240, 240), Color.BLACK);
        btnGenKeyEnc = createStyledButton("Sinh khóa", new Color(240, 240, 240), Color.BLACK);
        pnlKeyBtns.add(btnLoadKeyEnc);
        pnlKeyBtns.add(btnSaveKeyEnc);
        pnlKeyBtns.add(btnGenKeyEnc);
        gbc.gridx = 1;
        gbc.gridy = 3;
        panel.add(pnlKeyBtns, gbc);

        // Dòng 4: Định dạng đầu ra
        gbc.gridx = 0;
        gbc.gridy = 4;
        panel.add(new JLabel("Định dạng đầu ra:"), gbc);
        cbEncryptFormat = new JComboBox<>(new String[] { "Base64", "Hex" });
        gbc.gridx = 1;
        panel.add(cbEncryptFormat, gbc);

        // Dòng 5: Kết quả mã hóa
        gbc.gridx = 0;
        gbc.gridy = 5;
        panel.add(new JLabel("Kết quả mã hóa:"), gbc);
        txtEncryptedResult = new JTextArea(4, 25);
        txtEncryptedResult.setLineWrap(true);
        gbc.gridx = 1;
        panel.add(new JScrollPane(txtEncryptedResult), gbc);

        // Dòng 6: Các nút hành động chính
        JPanel pnlActionBtns = new JPanel(new FlowLayout(FlowLayout.CENTER, 20, 0));
        pnlActionBtns.setOpaque(false);
        btnEncrypt = createStyledButton("Mã hóa", new Color(240, 240, 240), Color.BLACK);
        btnClearEnc = createStyledButton("Xóa", new Color(240, 240, 240), Color.BLACK);
        btnSaveResultEnc = createStyledButton("Lưu kết quả", new Color(240, 240, 240), Color.BLACK);

        pnlActionBtns.add(btnEncrypt);
        pnlActionBtns.add(btnClearEnc);
        pnlActionBtns.add(btnSaveResultEnc);
        gbc.gridx = 0;
        gbc.gridy = 6;
        gbc.gridwidth = 2;
        panel.add(pnlActionBtns, gbc);

        return panel;
    }

    // ================== PANEL GIẢI MÃ ==================
    private JPanel createDecryptionPanel() {
        JPanel panel = new JPanel(new GridBagLayout());
        panel.setBorder(BorderFactory.createTitledBorder(null, "Giải mã", TitledBorder.CENTER, TitledBorder.TOP,
                new Font("Arial", Font.BOLD, 16), new Color(100, 150, 255)));
        panel.setBackground(new Color(190, 225, 255));

        GridBagConstraints gbc = new GridBagConstraints();
        gbc.insets = new Insets(8, 8, 8, 8);
        gbc.fill = GridBagConstraints.HORIZONTAL;

        // Dòng 1: Văn bản mã hóa
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.weightx = 0.1;
        panel.add(new JLabel("Văn bản mã hóa:"), gbc);
        txtEncryptedInput = new JTextArea(4, 25);
        txtEncryptedInput.setLineWrap(true);
        gbc.gridx = 1;
        gbc.weightx = 0.9;
        panel.add(new JScrollPane(txtEncryptedInput), gbc);

        // Nút tải tệp mã hóa
        JPanel pnlLoadEncText = new JPanel(new FlowLayout(FlowLayout.RIGHT, 0, 0));
        pnlLoadEncText.setOpaque(false);
        btnLoadTextDec = createStyledButton("Tải tệp mã hóa", new Color(200, 230, 255), Color.BLACK);
        pnlLoadEncText.add(btnLoadTextDec);
        gbc.gridx = 1;
        gbc.gridy = 1;
        panel.add(pnlLoadEncText, gbc);

        // Dòng 2: Nhập khóa (Đã bỏ Độ dài khóa)
        gbc.gridx = 0;
        gbc.gridy = 2;
        panel.add(new JLabel("Nhập khóa:"), gbc);
        txtDecryptKey = new JTextField("22F1E653B02CFFB4");
        gbc.gridx = 1;
        panel.add(txtDecryptKey, gbc);

        // Dòng 3: Nhóm nút cho Khóa giải mã
        JPanel pnlKeyBtnsDec = new JPanel(new FlowLayout(FlowLayout.CENTER, 10, 0));
        pnlKeyBtnsDec.setOpaque(false);
        btnLoadKeyDec = createStyledButton("Tải khóa", new Color(240, 240, 240), Color.BLACK);
        btnSaveKeyDec = createStyledButton("Lưu khóa", new Color(240, 240, 240), Color.BLACK);
        pnlKeyBtnsDec.add(btnLoadKeyDec);
        pnlKeyBtnsDec.add(btnSaveKeyDec);
        gbc.gridx = 1;
        gbc.gridy = 3;
        panel.add(pnlKeyBtnsDec, gbc);

        // Dòng 4: Định dạng đầu vào
        gbc.gridx = 0;
        gbc.gridy = 4;
        panel.add(new JLabel("Định dạng:"), gbc);
        cbDecryptFormat = new JComboBox<>(new String[] { "Base64", "Hex" });
        gbc.gridx = 1;
        panel.add(cbDecryptFormat, gbc);

        // Dòng 5: Kết quả giải mã
        gbc.gridx = 0;
        gbc.gridy = 5;
        panel.add(new JLabel("Kết quả giải mã:"), gbc);
        txtDecryptedResult = new JTextArea(4, 25);
        txtDecryptedResult.setLineWrap(true);
        gbc.gridx = 1;
        panel.add(new JScrollPane(txtDecryptedResult), gbc);

        // Dòng 6: Các nút hành động chính
        JPanel pnlActionBtnsDec = new JPanel(new FlowLayout(FlowLayout.CENTER, 20, 0));
        pnlActionBtnsDec.setOpaque(false);
        btnDecrypt = createStyledButton("Giải mã", new Color(240, 240, 240), Color.BLACK);
        btnClearDec = createStyledButton("Xóa", new Color(240, 240, 240), Color.BLACK);
        btnSaveResultDec = createStyledButton("Lưu kết quả", new Color(240, 240, 240), Color.BLACK);

        pnlActionBtnsDec.add(btnDecrypt);
        pnlActionBtnsDec.add(btnClearDec);
        pnlActionBtnsDec.add(btnSaveResultDec);
        gbc.gridx = 0;
        gbc.gridy = 6;
        gbc.gridwidth = 2;
        panel.add(pnlActionBtnsDec, gbc);

        return panel;
    }

    // ================== HÀM HỖ TRỢ TẠO NÚT ==================
    // Cập nhật hàm tạo nút để hiển thị rõ ràng trên mọi hệ điều hành
    private JButton createStyledButton(String text, Color bgColor, Color fgColor) {
        JButton btn = new JButton(text);
        btn.setBackground(bgColor);
        btn.setForeground(fgColor); // Đảm bảo chữ hiển thị rõ
        btn.setFocusPainted(false);

        // Cực kì quan trọng để nút không bị mờ (trong suốt) trên Windows LookAndFeel
        btn.setOpaque(true);
        btn.setContentAreaFilled(true);

        // Thêm viền để nút bấm trông gọn gàng
        btn.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createLineBorder(Color.GRAY, 1),
                BorderFactory.createEmptyBorder(5, 15, 5, 15)));

        btn.setFont(new Font("Arial", Font.BOLD, 12));
        return btn;
    }

    // ================== SỰ KIỆN XỬ LÝ (DES + FILE I/O) ==================
    private void addEventHandlers() {
        // --- XỬ LÝ NÚT MÃ HÓA ---
        btnEncrypt.addActionListener(e -> {
            try {
                String plainText = txtOriginal.getText();
                String hexKey = txtEncryptKey.getText().trim();

                if (plainText.isEmpty() || hexKey.isEmpty()) {
                    JOptionPane.showMessageDialog(this, "Vui lòng nhập văn bản và khóa!", "Lỗi",
                            JOptionPane.ERROR_MESSAGE);
                    return;
                }
                if (hexKey.length() != 16) {
                    JOptionPane.showMessageDialog(this, "Khóa phải có đúng 16 ký tự Hex (8 bytes)!", "Lỗi",
                            JOptionPane.ERROR_MESSAGE);
                    return;
                }

                byte[] keyBytes = hexStringToByteArray(hexKey);
                SecretKeySpec secretKey = new SecretKeySpec(keyBytes, "DES");
                Cipher cipher = Cipher.getInstance("DES/ECB/PKCS5Padding");
                cipher.init(Cipher.ENCRYPT_MODE, secretKey);

                byte[] encryptedBytes = cipher.doFinal(plainText.getBytes(StandardCharsets.UTF_8));
                String base64Result = Base64.getEncoder().encodeToString(encryptedBytes);

                txtEncryptedResult.setText(base64Result);
                JOptionPane.showMessageDialog(this, "✔ Mã hóa thành công.", "Thông báo",
                        JOptionPane.INFORMATION_MESSAGE);

            } catch (Exception ex) {
                JOptionPane.showMessageDialog(this, "Lỗi mã hóa: " + ex.getMessage(), "Lỗi", JOptionPane.ERROR_MESSAGE);
            }
        });

        // --- XỬ LÝ NÚT GIẢI MÃ ---
        btnDecrypt.addActionListener(e -> {
            try {
                String encryptedText = txtEncryptedInput.getText().trim();
                String hexKey = txtDecryptKey.getText().trim();

                if (encryptedText.isEmpty() || hexKey.isEmpty()) {
                    JOptionPane.showMessageDialog(this, "Vui lòng nhập bản mã và khóa!", "Lỗi",
                            JOptionPane.ERROR_MESSAGE);
                    return;
                }

                byte[] encryptedBytes = Base64.getDecoder().decode(encryptedText);
                byte[] keyBytes = hexStringToByteArray(hexKey);
                SecretKeySpec secretKey = new SecretKeySpec(keyBytes, "DES");
                Cipher cipher = Cipher.getInstance("DES/ECB/PKCS5Padding");
                cipher.init(Cipher.DECRYPT_MODE, secretKey);

                byte[] decryptedBytes = cipher.doFinal(encryptedBytes);
                txtDecryptedResult.setText(new String(decryptedBytes, StandardCharsets.UTF_8));
                JOptionPane.showMessageDialog(this, "✔ Giải mã thành công.", "Thông báo",
                        JOptionPane.INFORMATION_MESSAGE);

            } catch (Exception ex) {
                JOptionPane.showMessageDialog(this, "Lỗi giải mã: Khóa sai hoặc dữ liệu bị hỏng!", "Lỗi",
                        JOptionPane.ERROR_MESSAGE);
            }
        });

        // --- FILE I/O EVENT LISTENERS ---
        // Lưu file bản mã (Bước 2)
        btnSaveResultEnc.addActionListener(e -> saveToFile(txtEncryptedResult.getText()));

        // Tải file bản mã sang giải mã (Bước 3)
        btnLoadTextDec.addActionListener(e -> loadFromFile(txtEncryptedInput));

        // Lưu/Tải kết quả giải mã và văn bản gốc
        btnLoadTextEnc.addActionListener(e -> loadFromFile(txtOriginal));
        btnSaveResultDec.addActionListener(e -> saveToFile(txtDecryptedResult.getText()));

        // Các nút Clear
        btnClearEnc.addActionListener(e -> {
            txtOriginal.setText("");
            txtEncryptedResult.setText("");
        });
        btnClearDec.addActionListener(e -> {
            txtEncryptedInput.setText("");
            txtDecryptedResult.setText("");
        });
    }

    // ================== FILE I/O METHODS ==================
    private void loadFromFile(JTextArea targetArea) {
        JFileChooser fileChooser = new JFileChooser();
        if (fileChooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
            try {
                File file = fileChooser.getSelectedFile();
                String content = new String(Files.readAllBytes(file.toPath()), StandardCharsets.UTF_8);
                targetArea.setText(content);
            } catch (Exception ex) {
                JOptionPane.showMessageDialog(this, "Lỗi đọc file: " + ex.getMessage(), "Lỗi",
                        JOptionPane.ERROR_MESSAGE);
            }
        }
    }

    private void saveToFile(String content) {
        if (content == null || content.isEmpty()) {
            JOptionPane.showMessageDialog(this, "Không có dữ liệu để lưu!", "Cảnh báo", JOptionPane.WARNING_MESSAGE);
            return;
        }
        JFileChooser fileChooser = new JFileChooser();
        if (fileChooser.showSaveDialog(this) == JFileChooser.APPROVE_OPTION) {
            try {
                File file = fileChooser.getSelectedFile();
                Files.write(file.toPath(), content.getBytes(StandardCharsets.UTF_8));
                JOptionPane.showMessageDialog(this, "Đã lưu file thành công vào:\n" + file.getAbsolutePath(),
                        "Thông báo", JOptionPane.INFORMATION_MESSAGE);
            } catch (Exception ex) {
                JOptionPane.showMessageDialog(this, "Lỗi ghi file: " + ex.getMessage(), "Lỗi",
                        JOptionPane.ERROR_MESSAGE);
            }
        }
    }

    // ================== HÀM PHỤ TRỢ HEX ==================
    public static byte[] hexStringToByteArray(String s) {
        int len = s.length();
        byte[] data = new byte[len / 2];
        for (int i = 0; i < len; i += 2) {
            data[i / 2] = (byte) ((Character.digit(s.charAt(i), 16) << 4)
                    + Character.digit(s.charAt(i + 1), 16));
        }
        return data;
    }

    // ================== MAIN ==================
    public static void main(String[] args) {
        try {
            // Sử dụng LookAndFeel mặc định
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
        } catch (Exception e) {
            e.printStackTrace();
        }

        SwingUtilities.invokeLater(() -> new DESStringTool().setVisible(true));
    }
}
