import hashlib
import bencode
import sys

def get_info_hash(torrent_path):
    with open(torrent_path, 'rb') as f:
        torrent_data = bencode.bdecode(f.read())

    if 'info' not in torrent_data:
        raise KeyError("'info' key not found (library parsed keys as Strings)")
        
    info_dict = torrent_data['info']
    info_bytes = bencode.bencode(info_dict)

    return hashlib.sha1(info_bytes).hexdigest()

if __name__ == "__main__":
    filename = "./assets/ubuntu-24.04.1-live-server-amd64.iso.torrent"
    
    try:
        calculated_hash = get_info_hash(filename)
        print(f"Calculated Hash: {calculated_hash}")
        
    except Exception as e:
        print(f"Error: {e}")
