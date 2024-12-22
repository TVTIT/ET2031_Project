# Phần mềm Quản lý hệ thống vay nợ
Đây là bài tập lớn học phần ET2031 - Kỹ thuật lập trình C/C++ của nhóm 3, lớp 156396, học kì 2024.1

---

## Mục lục
- [Giới thiệu chung](#giới-thiệu-chung)
- [Tính năng chính](#tính-năng-chính)
- [Yêu cầu hệ thống](#yêu-cầu-hệ-thống)
- [Cách cài đặt](#cách-cài-đặt)
- [Hướng dẫn sử dụng phần mềm](#hướng-dẫn-sử-dụng-phần-mềm)
    - [Giao diện chính](#giao-diện-chính)
    - [Giao diện Khách hàng](#giao-diện-khách-hàng)
    - [Giao diện Khoản vay](#giao-diện-khoản-vay)
- [Thư viện sử dụng trong dự án](#thư-viện-sử-dụng-trong-dự-án)
- [Về chúng tôi](#về-chúng-tôi)

---

## Giới thiệu chung

Phần mềm **Quản lý hệ thống vay nợ** hướng tới đối tượng chính là những người cho vay, bao gồm cá nhân cho vay nhỏ lẻ, chủ các tổ chức tín dụng vi mô, và những người cần công cụ hỗ trợ theo dõi danh sách người vay, quản lý khoản nợ và lịch trả nợ một cách hiệu quả.

Mục tiêu của ứng dụng là:
- Hỗ trợ quản lý các khoản vay, theo dõi tình trạng trả nợ, và tính toán lãi suất.
- Lưu trữ thông tin người vay chi tiết và an toàn.
- Tối ưu hóa quy trình quản lý, giảm thiểu sai sót thủ công, và tiết kiệm thời gian.

---

## Tính năng chính
- Quản lý thông tin người vay:
    - Lưu trữ thông tin cơ bản: Họ và tên, số điện thoại, địa chỉ.
    - Thêm mới, sửa đổi, và xóa thông tin người vay.
- Quản lý khoản vay:
    - Thông tin khoản vay bao gồm: mã khoản vay, số tiền vay, lãi suất, thời hạn.
    - Hỗ trợ tính toán số tiền phải trả hàng tháng, tổng số tiền phải trả và lãi suất tích lũy.
    - Cập nhật trạng thái khoản vay.
- Báo cáo và thống kê:
    - Hiển thị danh sách khoản vay (đang vay, đã trả, quá hạn).
    - Tổng hợp: tổng số tiền cho vay, số tiền đã thu hồi, khoản nợ quá hạn.
- Tìm kiếm và sắp xếp:
    - Tìm kiếm người vay theo số CCCD.
    - Tìm kiếm khoản vay theo mã khoản vay hoặc số CCCD của người vay.

---

## Yêu cầu hệ thống
- Máy tính chạy hệ điều hành Windows 64-bit (khuyến nghị Windows 10 trở lên).

---

## Cách cài đặt
Phần mềm không yêu cầu cài đặt. Bạn chỉ cần:
1. Tải phiên bản mới nhất tại phần [Release](https://github.com/TVTIT/ET2031_Project/releases/latest).
2. Chạy file vừa tải.

> **Lưu ý**: Phần mềm sẽ tạo ra 2 file `customers_list.csv` và `loans_list.csv` để lưu dữ liệu. Vì vậy, hãy đặt phần mềm trong một thư mục riêng để quản lý dễ dàng.

---


## Hướng dẫn sử dụng phần mềm

### Giao diện chính
![Ảnh giao diện chính](/img/Giao%20diện%20chính.png)

Khởi động phần mềm. Giao diện chính sẽ có các lựa chọn sau:
- **1**: Khách hàng
- **2**: Khoản vay
- **3**: Thoát  

Nhập số thứ tự tương ứng và nhấn Enter.

### Giao diện Khách hàng
![Ảnh giao diện khách hàng](/img/Giao%20diện%20khách%20hàng.png)

Tại đây, bạn có các lựa chọn:
1. **Thêm khách hàng**: Nhập thông tin chi tiết bao gồm tên, ngày sinh, SĐT, địa chỉ, số CCCD.  
   ![Ảnh thêm khách hàng](/img/Thêm%20khách%20hàng.png)
2. **Tìm khách hàng theo số CCCD**: Thao tác với khách hàng gồm sửa, xóa, thêm khoản vay hoặc liệt kê khoản vay.  
   ![Ảnh tìm khách hàng](/img/Tìm%20kiếm%20khách%20hàng.png)
3. **Xóa khách hàng**.
4. **Quay lại màn hình chính**.

### Giao diện Khoản vay
![Ảnh giao diện khoản vay](/img/Giao%20diện%20khoản%20vay.png)

Các lựa chọn trong giao diện này bao gồm:
1. **Thêm khoản vay**: Nhập thông tin số tiền vay, ngày vay, thời hạn, lãi suất, ghi chú.  
   ![Ảnh thêm khoản vay](/img/Thêm%20khoản%20vay.png)
2. **Tìm khoản vay theo số CCCD**.  
   ![Ảnh tìm khoản vay theo số CCCD](/img/Tìm%20kiểm%20khoản%20vay%20theo%20CCCD.png)
3. **Tìm khoản vay theo mã khoản vay** và thao tác với khoản vay (sửa, xóa, liệt kê lịch sử nộp tiền, tính số tiền cố định mỗi tháng phải trả).  
   ![Ảnh tìm khoản vay theo mã khoản vay](/img/TÌm%20kiếm%20khoản%20vay%20theo%20mã%20khoản%20vay.png)
4. **Liệt kê khoản vay hết hạn** và tùy chọn xóa tất cả các khoản vay đó.
5. **Liệt kê khoản vay đã trả hết nợ** và tùy chọn xóa tất cả các khoản vay đó.
6. **Liệt kê các khoản vay lâu chưa đóng tiền**: Liệt kê dựa trên số tháng người dùng nhập.
7. **Thống kê tổng quan**: Tính toán tổng số tiền cho vay, lãi phát sinh, và số tiền thu hồi dự kiến.
8. **Quay lại màn hình chính**.

## Thư viện sử dụng trong dự án
- [boost](https://www.boost.org/)
- [fmt](https://fmt.dev/)
- [rapidcsv](https://github.com/d99kris/rapidcsv)

Cảm ơn những tác giả, người đóng góp cho 3 thư viện trên đã giúp chúng tôi hoàn thành bài tập lớn này

## Về chúng tôi
Dự án này được thiết kế và xây dựng bởi nhóm 7, lớp 156396 môn Kỹ thuật lập trình C/C++ (ET2031) của Trường Điện - Điện tử, Đại học Bách khoa Hà Nội học kì 2024.1. Nhóm chúng tôi bao gồm:
- Đỗ Hữu Ánh
- Trịnh Thuý Ngân
- Lê Minh Thái
- Trần Vĩnh Trung
