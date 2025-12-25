import hashlib
import bencode
import sys

# The expected hash for Ubuntu 24.04.1 Live Server
EXPECTED_HASH = "5a8e6308665790d96d95304918a38a79853380e5"

def get_info_hash(torrent_path):
    with open(torrent_path, 'rb') as f:
        torrent_data = bencode.bdecode(f.read())

    # FIX: Use string 'info' instead of bytes b'info'
    # The library you are using has already decoded the keys to strings
    if 'info' not in torrent_data:
        raise KeyError("'info' key not found (library parsed keys as Strings)")
        
    info_dict = torrent_data['info']

    # Re-encode only the info dictionary to get the raw bytes for hashing
    info_bytes = bencode.bencode(info_dict)

    return hashlib.sha1(info_bytes).hexdigest()

if __name__ == "__main__":
    filename = "./assets/ubuntu-24.04.1-live-server-amd64.iso.torrent"
    
    try:
        calculated_hash = get_info_hash(filename)
        print(f"Calculated Hash: {calculated_hash}")
        
        if calculated_hash == EXPECTED_HASH:
            print("✅ SUCCESS! Matches official Ubuntu hash.")
        else:
            print("❌ MISMATCH.") 
            print(f"Expected: {EXPECTED_HASH}")
            print("Note: If mismatch occurs, your bencode library might be corrupting binary data inside 'pieces'.")
            
    except Exception as e:
        print(f"Error: {e}")
