from fastapi import FastAPI, File, UploadFile, HTTPException
from typing import List

app = FastAPI()

@app.post("/upload/")
async def upload_files(files: List[UploadFile] = File(...)):
    for file in files:
        if not file.content_type.startswith("image/jpeg"):
            raise HTTPException(status_code=400, detail="Solo se permiten archivos JPG")

        contents = await file.read()
        # Aquí puedes guardar la imagen en el sistema de archivos
        # Por ejemplo, puedes guardarla con un nombre aleatorio
        with open(f"imagen_{file.filename}", "wb") as f:
            f.write(contents)

    return {"files_uploaded": [file.filename for file in files]}
