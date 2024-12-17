import base64
from pathlib import Path

file_path=Path(__file__).parent.resolve()

(file_path/"bomb").write_bytes(base64.b64decode((file_path/"bomb.base64").read_text()))
