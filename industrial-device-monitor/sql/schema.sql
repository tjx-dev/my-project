-- 创建项目数据库和温度历史表，可重复执行。
CREATE DATABASE IF NOT EXISTS device_monitor CHARACTER SET utf8mb4;
USE device_monitor;

-- 每收到一条有效记录，就新增一行。
CREATE TABLE IF NOT EXISTS temperature_readings (
    id BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    device_id VARCHAR(16) NOT NULL,
    temperature SMALLINT NOT NULL,
    received_at TIMESTAMP(3) NOT NULL DEFAULT CURRENT_TIMESTAMP(3),
    INDEX idx_device_time (device_id, received_at)
) ENGINE=InnoDB;

INSERT INTO temperature_readings (device_id, temperature)
VALUES ('E', 85);

INSERT INTO temperature_readings (device_id, temperature)
VALUES ('E', 84);

select *
from temperature_readings;

select * from temperature_readings where device_id="E" order by id DESC ;




