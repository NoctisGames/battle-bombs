package com.technegames.battlebombs;

import java.io.BufferedOutputStream;
import java.io.Closeable;
import java.io.File;
import java.io.FileOutputStream;
import java.io.Flushable;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.zip.ZipInputStream;

public final class IOUtils
{
    private static final Logger logger = new Logger(IOUtils.class);

    public static void writeBytesToPath(byte[] bytes, String path)
    {
        final File file = new File(path);
        BufferedOutputStream bos = null;
        try
        {
            bos = new BufferedOutputStream(new FileOutputStream(file));
            bos.write(bytes);
            IOUtils.safeFlush(bos);
        }
        catch (IOException e)
        {
            logger.error("IOException occurred while writing to: " + path + ", " + e.toString(), e);
        }
        finally
        {
            IOUtils.safeClose(bos);
        }
    }

    public static void safeFlush(Flushable flushable)
    {
        if (flushable != null)
        {
            try
            {
                flushable.flush();
            }
            catch (Exception e)
            {
                logger.error("Error flushing " + flushable.getClass().getSimpleName(), e);
            }
        }
    }

    public static void safeClose(Closeable closeable)
    {
        if (closeable != null)
        {
            try
            {
                closeable.close();
            }
            catch (Exception e)
            {
                logger.error("Error closing " + closeable.getClass().getSimpleName(), e);
            }
        }
    }

    public static void safeCloseEntry(ZipInputStream zipInputStream)
    {
        if (zipInputStream != null)
        {
            try
            {
                zipInputStream.closeEntry();
            }
            catch (Exception e)
            {
                logger.error("Error closing entry in ZipInputStream", e);
            }
        }
    }

    public static String extractStringFromInputStream(InputStream inputStream) throws IOException
    {
        byte[] buffer = new byte[1024];

        StringBuilder sb = new StringBuilder();

        int bufferLength;
        while ((bufferLength = inputStream.read(buffer)) != -1)
        {
            sb.append(new String(buffer, 0, bufferLength));
        }

        return sb.toString();
    }

    public static void pipeStreams(InputStream inputStream, OutputStream outputStream) throws IOException
    {
        byte[] buffer = new byte[1024];

        int read;
        while ((read = inputStream.read(buffer)) != -1)
        {
            if (read == 0)
            {
                continue;
            }

            outputStream.write(buffer, 0, read);
        }
    }

    private IOUtils()
    {
        // Hide Constructor
    }
}