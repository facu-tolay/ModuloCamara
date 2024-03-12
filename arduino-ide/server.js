const http = require('http');
const fs = require('fs');

const server = http.createServer((req, res) => {
  if (req.method === 'POST') {
    let imageData = '';

    req.on('data', chunk => {
      imageData += chunk;
    });

    req.on('end', () => {
      // Aquí puedes guardar la imagen en el sistema de archivos
      // Por ejemplo, puedes guardarla con un nombre aleatorio
      const imageName = `imagen_${Date.now()}.jpg`;
      fs.writeFile(imageName, imageData, 'base64', err => {
        if (err) {
          console.error(err);
          res.writeHead(500, { 'Content-Type': 'application/json' });
          res.end(JSON.stringify({ error: 'Error al guardar la imagen' }));
        } else {
          console.log(`Imagen guardada como ${imageName}`);
          res.writeHead(200, { 'Content-Type': 'application/json' });
          res.end(JSON.stringify({ message: 'Imagen recibida y guardada correctamente' }));
        }
      });
    });
  } else {
    res.writeHead(405, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify({ error: 'Método no permitido' }));
  }
});


const PORT = process.env.PORT || 3000;

server.listen(PORT, '0.0.0.0', () => {
  console.log(`Servidor escuchando en el puerto ${PORT}`);
});
