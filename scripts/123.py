import os
import chardet

def convert_to_utf8(folder_path):
    # 定义要转换的文件格式
    file_extensions = ['.cpp', '.h', '.pro', '.pri', '.ini','.txt']
    
    for filename in os.listdir(folder_path):
        # 检查文件扩展名是否在定义的格式中
        if any(filename.endswith(ext) for ext in file_extensions):
            file_path = os.path.join(folder_path, filename)
            
            # 确保是文件而不是文件夹
            if os.path.isfile(file_path):
                try:
                    with open(file_path, 'rb') as f:
                        raw_data = f.read()
                    detected_encoding = chardet.detect(raw_data)['encoding']
                    
                    # 如果文件不是UTF-8编码，则进行转换
                    if detected_encoding != 'utf-8':
                        content = raw_data.decode(detected_encoding, errors='ignore').encode('utf-8')
                        with open(file_path, 'wb') as f:
                            f.write(content)
                        print(f"Converted {filename} to UTF-8 encoding.")
                except Exception as e:
                    print(f"Error processing {filename}: {e}")

# 指定要转换的文件夹路径
folder_path = './'
convert_to_utf8(folder_path)