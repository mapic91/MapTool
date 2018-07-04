using System;
using System.Collections.Generic;
using System.IO;
using TiledSharp;

namespace TmxReadHelper
{
    public class TmxReader
    {
        private const string Layer1Name = "1";
        private const string Layer2Name = "2";
        private const string Layer3Name = "3";
        private const string TrapName = "T";
        private const string ObstacleName = "O";

        public static Proto.TmxMap ReadTmxMap(string path)
        {
            try
            {
                var tmx = new TmxMap(path);
                var tmxOut = new Proto.TmxMap();
                tmxOut.Width = tmx.Width;
                tmxOut.Height = tmx.Height;
                var imgs = LoadImgs(tmx, tmxOut);
                var barrerGidToType = new Dictionary<int, Proto.TmxMap.Types.BarrerType>();
                var trapGidToIndex = new Dictionary<int, int>();
                LoadTileImgs(tmx, tmxOut, imgs, barrerGidToType, trapGidToIndex);
                LoadTileLayersAndSettings(tmx, tmxOut, barrerGidToType, trapGidToIndex);

                return tmxOut;
            }
            catch (Exception e)
            {
                return null;
            }
        }

        private static void LoadTileLayersAndSettings(TmxMap tmx, Proto.TmxMap tmxOut, Dictionary<int, Proto.TmxMap.Types.BarrerType> barrerGidToType, Dictionary<int, int> trapGidToIndex)
        {
            TmxLayer[] layers = new TmxLayer[3];
            TmxLayer trapLayer = null, obstacleLayer = null; 
            foreach (var layer in tmx.Layers)
            {
                switch (layer.Name)
                {
                    case Layer1Name:
                        layers[0] = layer;
                        break;
                    case Layer2Name:
                        layers[1] = layer;
                        break;
                    case Layer3Name:
                        layers[2] = layer;
                        break;
                    case TrapName:
                        trapLayer = layer;
                        break;
                    case ObstacleName:
                        obstacleLayer = layer;
                        break;
                }
            }

            //Fix trap layer setting error.
            if (trapLayer != null)
            {
                foreach (var tile in trapLayer.Tiles)
                {
                    if (tile.Gid != 0 && !trapGidToIndex.ContainsKey(tile.Gid))
                    {
                        trapGidToIndex[tile.Gid] = 0;
                    }
                }
            }

            //Fix obstacle layer setting error.
            if (obstacleLayer != null)
            {
                foreach (var tile in obstacleLayer.Tiles)
                {
                    if (tile.Gid != 0 && !barrerGidToType.ContainsKey(tile.Gid))
                    {
                        barrerGidToType[tile.Gid] = Proto.TmxMap.Types.BarrerType.None;
                    }
                }
            }

            for (int i = 0; i < 3; i++)
            {
                var layer = layers[i];
                if (layer != null)
                {
                    var l = new Proto.TmxMap.Types.Layer();
                    l.Index = i;
                    foreach (var tile in layer.Tiles)
                    {
                        var index = tile.X + tile.Y * tmx.Width;
                        l.Tiles.Add(new Proto.TmxMap.Types.TileInfo()
                        {
                            Gid = tile.Gid,
                            X = tile.X,
                            Y = tile.Y,
                        });
                    }
                    tmxOut.Layers.Add(l);
                }
            }

            for (int i = 0; i < tmx.Width*tmx.Height; i++)
            {
                var barrerGid = obstacleLayer == null ? 0 : obstacleLayer.Tiles[i].Gid;
                var trapGid = trapLayer == null ? 0 : trapLayer.Tiles[i].Gid;
                tmxOut.Settings.Add(new Proto.TmxMap.Types.TileSetting()
                {
                    Barrertype = barrerGid == 0 ? Proto.TmxMap.Types.BarrerType.None : barrerGidToType[barrerGid],
                    Trapindex = trapGid == 0 ? 0 : trapGidToIndex[trapGid]
                });
            }
        }

        private static void LoadTileImgs(TmxMap tmx, Proto.TmxMap tmxOut, Dictionary<string, int> imgs,
            Dictionary<int, Proto.TmxMap.Types.BarrerType> barrerGidToType, Dictionary<int, int> trapGidToIndex)
        {
            //_tileTextureInfos = new TileTextureInfo[_tileTextureCounts + 1]; //tile texture gid start at 1
            tmxOut.Tileimgs.Add(new Proto.TmxMap.Types.TileImg()); //tile img gid start at 1
            foreach (var tileset in tmx.Tilesets)
            {
                var firstGid = tileset.FirstGid;
                switch (tileset.Name)
                {
                    case TrapName:
                    {
                        foreach (var tile in tileset.Tiles)
                        {
                            trapGidToIndex[firstGid + tile.Id] =
                                int.Parse(Path.GetFileNameWithoutExtension(tile.Image.Source));
                        }

                        break;
                    }
                    case ObstacleName:
                    {
                        foreach (var tile in tileset.Tiles)
                        {
                            switch (Path.GetFileNameWithoutExtension(tile.Image.Source))
                            {
                                case "跳透":
                                    barrerGidToType[firstGid + tile.Id] = Proto.TmxMap.Types.BarrerType.CanOverTrans;
                                    break;
                                case "跳障":
                                    barrerGidToType[firstGid + tile.Id] = Proto.TmxMap.Types.BarrerType.CanOverObstacle;
                                    break;
                                case "透":
                                    barrerGidToType[firstGid + tile.Id] = Proto.TmxMap.Types.BarrerType.Trans;
                                    break;
                                case "障":
                                    barrerGidToType[firstGid + tile.Id] = Proto.TmxMap.Types.BarrerType.Obstacle;
                                    break;
                            }
                        }
                        break;
                    }
                }

                if (string.IsNullOrEmpty(tileset.Image.Source))
                {
                    foreach (var tile in tileset.Tiles)
                    {
                        var info = new Proto.TmxMap.Types.TileImg();
                        info.Gid = firstGid + tile.Id;
                        info.Imgindex = imgs[tile.Image.Source];
                        info.Region = new Proto.TmxMap.Types.Rect();
                        info.Region.X = 0;
                        info.Region.Y = 0;
                        info.Region.W = tile.Image.Width.Value;
                        info.Region.H = tile.Image.Height.Value;
                        info.Offset = new Proto.TmxMap.Types.Point();
                        info.Offset.X = tileset.TileOffset.X;
                        info.Offset.Y = tileset.TileOffset.Y;
                        tmxOut.Tileimgs.Add(info);
                    }
                }
                else
                {
                    var columns = tileset.Columns.Value;
                    var count = 0;
                    var row = 0;
                    while (true)
                    {
                        for (var i = 0; i < columns; i++)
                        {
                            var info = new Proto.TmxMap.Types.TileImg();
                            info.Gid = firstGid + i;
                            info.Imgindex = imgs[tileset.Image.Source];
                            info.Region = new Proto.TmxMap.Types.Rect();
                            info.Region.X = tileset.Margin + i*(tileset.Spacing + tileset.TileWidth);
                            info.Region.Y = tileset.Margin + row*(tileset.Spacing + tileset.TileHeight);
                            info.Region.W = tileset.TileWidth;
                            info.Region.H = tileset.TileHeight;
                            info.Offset = new Proto.TmxMap.Types.Point();
                            info.Offset.X = tileset.TileOffset.X;
                            info.Offset.Y = tileset.TileOffset.Y;
                            tmxOut.Tileimgs.Add(info);
                            count++;
                        }
                        if (count == tileset.TileCount.Value) break;
                        row++;
                    }
                }
            }
        }

        private static Dictionary<string,int> LoadImgs(TmxMap tmx, Proto.TmxMap tmxOut)
        {
            var counts = 0;
            var imgs = new Dictionary<string,int>();
            foreach (var tileset in tmx.Tilesets)
            {      
                if (!string.IsNullOrEmpty(tileset.Image.Source))
                {
                    var path = tileset.Image.Source;
                    if (!imgs.ContainsKey(path))
                    {
                        tmxOut.Imgs.Add(new Proto.TmxMap.Types.ImgSet()
                        {
                            Path = Path.GetFullPath(path)
                        });
                        imgs[path] = counts++;
                    }
                }
                foreach (var tile in tileset.Tiles)
                {
                    if (!string.IsNullOrEmpty(tile.Image.Source))
                    {
                        var path = tile.Image.Source;
                        if (!imgs.ContainsKey(path))
                        {
                            tmxOut.Imgs.Add(new Proto.TmxMap.Types.ImgSet()
                            {
                                Path = Path.GetFullPath(path)
                            });
                            imgs[path] = counts++;
                        }
                    }
                }
            }
            return imgs;
        }
    }
}