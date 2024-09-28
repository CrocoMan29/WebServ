<?php
// print_r ($_FILES);
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // $name = $_POST["name"];
    // $email = $_POST["email"];

    $uploadDir = "./uploads/";
    if (!file_exists($uploadDir))
        mkdir($uploadDir, 0777, true);
    $uploadedFiles = [];
    foreach ($_FILES["media"]["tmp_name"] as $key => $tmpName) {
        $randomString = bin2hex(random_bytes(8));
        $uploadFile = $uploadDir . $randomString . "_" . basename($_FILES["media"]["name"][$key]);

        if (move_uploaded_file($tmpName, $uploadFile)) {
            $uploadedFiles[] = $uploadFile;
        }
    }

    if (!empty($uploadedFiles)) {
        echo "<h1>Upload Successful</h1>";
        // echo "<p>Name: $name</p>";
        // echo "<p>Email: $email</p>";
        echo "<p>Media Files:</p>";

        foreach ($uploadedFiles as $file) {
            $fileType = mime_content_type($file);
            if (strpos($fileType, 'image/') === 0) {
                echo "<img src='$file' alt='Uploaded Image' width='300'><br><br>";
            } else if (strpos($fileType, 'video/') === 0) {
                echo "<video controls width='300'><source src='$file' type='$fileType'>Your browser does not support the video tag.</video><br><br>";
            }
        }
    } else {
        echo "<h1>Upload Failed</h1>";
    }
}
?>

<form action="" method="POST" enctype="multipart/form-data">
    <!-- <label for="name">Name:</label>
    <input type="text" name="name" required><br><br> -->
    
    <!-- <label for="email">Email:</label>
    <input type="email" name="email" required><br><br> -->
    
    <label for="media">Media (Images and Videos):</label>
    <input type="file" name="media[]" accept="image/*,video/*" required><br><br>
    <input type="submit" value="Upload">
</form>